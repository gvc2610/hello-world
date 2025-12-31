/*
 * LRU Cache Implementation in C
 * =============================
 *
 * Theory of Operation:
 * --------------------
 * An LRU (Least Recently Used) Cache organizes items in order of use, allowing us to quickly identify
 * which item hasn't been used for the longest time. When the cache reaches its capacity, the
 * least recently used item is evicted to make room for new items.
 *
 * This implementation uses two primary data structures working in tandem:
 *
 * 1. Doubly Linked List:
 *    - Stores the actual cache items (Key, Value).
 *    - Maintains the order of usage.
 *    - The HEAD of the list represents the Most Recently Used (MRU) item.
 *    - The TAIL of the list represents the Least Recently Used (LRU) item.
 *    - Moving a node to the head or removing the tail is an O(1) operation.
 *
 * 2. Hash Table (with Chaining):
 *    - Maps a Key to the corresponding Node in the Doubly Linked List.
 *    - Allows for O(1) average time complexity for lookups (get) and updates (put).
 *    - Uses separate chaining (linked lists) to resolve hash collisions.
 *
 * ASCII Diagram:
 * --------------
 *
 *      Hash Table (Array of HashNodes)             Doubly Linked List (Cache Items)
 *      +---+                                       +------+     +------+     +------+
 *   0  | •-|-------------------------------------> | Node | <-> | Node | <-> | Node |
 *      +---+                                       | K:1  |     | K:5  |     | K:2  |
 *   1  | / | (NULL)                                | V:10 |     | V:50 |     | V:20 |
 *      +---+                                       +------+     +------+     +------+
 *   2  | •-|--------+                                 ^                        ^
 *      +---+        |                                 |                        |
 *      ...          |                              HEAD (MRU)               TAIL (LRU)
 *                   |
 *                   v
 *                +----------+
 *                | HashNode | (Collision Chain)
 *                | Key: 5   |
 *                | Node *---|-----> (Points to Node K:5)
 *                +----------+
 *
 * Operations:
 * -----------
 * - GET(key):
 *   1. Look up key in Hash Table -> Get Node pointer.
 *   2. If found, move Node to HEAD of Linked List (mark as most recently used).
 *   3. Return value.
 *
 * - PUT(key, value):
 *   1. Look up key in Hash Table.
 *   2. If exists: Update value, move Node to HEAD.
 *   3. If new:
 *      a. Create new Node, add to HEAD.
 *      b. Add mapping to Hash Table.
 *      c. If capacity exceeded: Remove TAIL Node (LRU), remove from Hash Table, free memory.
 */

#include <stdio.h>
#include <stdlib.h>

// ---------------------------------------------------------------------------
// Data Structures
// ---------------------------------------------------------------------------

// Doubly Linked List Node
// Stores the actual data and links to maintain LRU order.
typedef struct Node {
    int key;            // Cache Key
    int value;          // Cache Value
    struct Node *prev;  // Pointer to previous node (towards Head)
    struct Node *next;  // Pointer to next node (towards Tail)
} Node;

// Hash Map Node (for chaining)
// Used in the Hash Table to map keys to Linked List Nodes.
typedef struct HashNode {
    int key;                // Key (needed to verify match in collision chain)
    Node *cacheNode;        // Pointer to the actual data Node in the Doubly Linked List
    struct HashNode *next;  // Pointer to next HashNode (for collision handling)
} HashNode;

// LRU Cache Structure
// The main container for the cache.
typedef struct LRUCache {
    int capacity;           // Maximum number of items
    int size;               // Current number of items
    Node *head;             // Most Recently Used (MRU) item
    Node *tail;             // Least Recently Used (LRU) item
    HashNode **hashTable;   // Array of pointers to HashNodes
    int hashSize;           // Size of the hash table array
} LRUCache;

// ---------------------------------------------------------------------------
// Helper Functions
// ---------------------------------------------------------------------------

// Create a new Doubly Linked List Node
Node* createNode(int key, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->value = value;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// Create a new Hash Node
HashNode* createHashNode(int key, Node* cacheNode) {
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->key = key;
    newNode->cacheNode = cacheNode;
    newNode->next = NULL;
    return newNode;
}

// Initialize LRU Cache
LRUCache* createCache(int capacity) {
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    
    // Initialize Hash Table
    // We size the hash table larger than capacity (2x) to reduce the chance of collisions,
    // improving average lookup performance.
    cache->hashSize = capacity * 2; 
    cache->hashTable = (HashNode**)calloc(cache->hashSize, sizeof(HashNode*));
    
    return cache;
}

// Simple Hash Function
// Maps a key to an index in the hash table array.
int hash(LRUCache* cache, int key) {
    return abs(key) % cache->hashSize;
}

// ---------------------------------------------------------------------------
// Hash Table Operations
// ---------------------------------------------------------------------------

// Add a mapping (Key -> Node) to the Hash Table
void addToHash(LRUCache* cache, int key, Node* node) {
    int index = hash(cache, key);
    HashNode* newNode = createHashNode(key, node);
    
    // Insert at the beginning of the collision chain (simplest approach)
    newNode->next = cache->hashTable[index];
    cache->hashTable[index] = newNode;
}

// Remove a mapping from the Hash Table
void removeFromHash(LRUCache* cache, int key) {
    int index = hash(cache, key);
    HashNode* current = cache->hashTable[index];
    HashNode* prev = NULL;

    // Traverse the collision chain to find the key
    while (current != NULL) {
        if (current->key == key) {
            if (prev == NULL) {
                // Removing the first item in the chain
                cache->hashTable[index] = current->next;
            } else {
                // Removing from the middle or end
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Get the Doubly Linked List Node associated with a Key
Node* getFromHash(LRUCache* cache, int key) {
    int index = hash(cache, key);
    HashNode* current = cache->hashTable[index];
    
    // Traverse collision chain
    while (current != NULL) {
        if (current->key == key) {
            return current->cacheNode;
        }
        current = current->next;
    }
    return NULL; // Not found
}

// ---------------------------------------------------------------------------
// Linked List Operations
// ---------------------------------------------------------------------------

// Add a Node to the front of the list (Mark as MRU)
void addToHead(LRUCache* cache, Node* node) {
    node->next = cache->head;
    node->prev = NULL;
    
    if (cache->head != NULL) {
        cache->head->prev = node;
    }
    
    cache->head = node;
    
    // If list was empty, head is also tail
    if (cache->tail == NULL) {
        cache->tail = node;
    }
}

// Remove a Node from the list (does not free memory)
void removeNode(LRUCache* cache, Node* node) {
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        // Node was head
        cache->head = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        // Node was tail
        cache->tail = node->prev;
    }
}

// Move an existing node to the head (Update usage)
void moveToHead(LRUCache* cache, Node* node) {
    removeNode(cache, node);
    addToHead(cache, node);
}

// Remove the Least Recently Used item (Tail)
// Returns the node so the caller can free it and remove from hash map
Node* removeTail(LRUCache* cache) {
    Node* node = cache->tail;
    if (node != NULL) {
        removeNode(cache, node);
    }
    return node;
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

// Get value from cache
// Returns -1 if not found. Updates usage (moves to head) if found.
int get(LRUCache* cache, int key) {
    Node* node = getFromHash(cache, key);
    if (node == NULL) {
        return -1; // Not found
    }
    // Found: Move to head to mark as recently used
    moveToHead(cache, node);
    return node->value;
}

// Put value into cache
// Inserts new item or updates existing. Evicts LRU if full.
void put(LRUCache* cache, int key, int value) {
    Node* node = getFromHash(cache, key);

    if (node != NULL) {
        // Case 1: Key exists
        // Update value and move to head (MRU)
        node->value = value;
        moveToHead(cache, node);
    } else {
        // Case 2: Key does not exist
        Node* newNode = createNode(key, value);
        
        if (cache->size >= cache->capacity) {
            // Cache full: Evict LRU (Tail)
            Node* tail = removeTail(cache);
            removeFromHash(cache, tail->key);
            free(tail);
            cache->size--;
        }
        
        // Add new node to Head and Hash Table
        addToHead(cache, newNode);
        addToHash(cache, key, newNode);
        cache->size++;
    }
}

// Print Cache state (for debugging)
void printCache(LRUCache* cache) {
    Node* current = cache->head;
    printf("Cache (Head/MRU -> Tail/LRU): ");
    while (current != NULL) {
        printf("[%d:%d] ", current->key, current->value);
        current = current->next;
    }
    printf("\n");
}

// Free the cache and all allocated memory
void freeCache(LRUCache* cache) {
    // 1. Free Linked List Nodes
    Node* current = cache->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }

    // 2. Free Hash Table Buckets
    for (int i = 0; i < cache->hashSize; i++) {
        HashNode* hashNode = cache->hashTable[i];
        while (hashNode != NULL) {
            HashNode* temp = hashNode;
            hashNode = hashNode->next;
            free(temp);
        }
    }
    
    // 3. Free Array and Cache Structure
    free(cache->hashTable);
    free(cache);
}

// ---------------------------------------------------------------------------
// Main Driver
// ---------------------------------------------------------------------------
int main() {
    printf("Initializing LRU Cache with capacity 3...\n");
    LRUCache* cache = createCache(3);

    printf("\n--- Adding Elements ---\n");
    printf("Put (1, 10)\n");
    put(cache, 1, 10);
    printCache(cache);

    printf("Put (2, 20)\n");
    put(cache, 2, 20);
    printCache(cache);

    printf("Put (3, 30)\n");
    put(cache, 3, 30);
    printCache(cache);

    printf("\n--- Accessing Elements (Updates MRU) ---\n");
    printf("Get (1) -> %d\n", get(cache, 1));
    printCache(cache);

    printf("\n--- Adding Element when Full (Eviction) ---\n");
    printf("Put (4, 40) (Should evict LRU: 2)\n");
    put(cache, 4, 40);
    printCache(cache);

    printf("\n--- Verifying Eviction ---\n");
    printf("Get (2) -> %d (Expected: -1)\n", get(cache, 2)); 
    
    printf("\n--- Another Eviction ---\n");
    printf("Put (5, 50) (Should evict LRU: 3)\n");
    put(cache, 5, 50);
    printCache(cache);

    printf("\nCleaning up...\n");
    freeCache(cache);
    return 0;
}
