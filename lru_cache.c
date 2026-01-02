/*
 * LRU Cache Implementation in C
 * =============================
 *
 * Overview:
 * ---------
 * An LRU (Least Recently Used) Cache is a fixed-size data structure that
 * automatically removes the least recently accessed items when it reaches capacity.
 *
 * Data Structures:
 * ----------------
 * 1. Doubly Linked List (DLL):
 *    - Purpose: Maintains the order of items based on usage.
 *    - Head: Most Recently Used (MRU) item.
 *    - Tail: Least Recently Used (LRU) item.
 *    - Complexity: Moving a node to the head is O(1) if we have a pointer to it.
 *
 * 2. Hash Table (Chaining):
 *    - Purpose: Provides O(1) average time complexity for lookups.
 *    - Mapping: Key -> Pointer to the corresponding DLL Node.
 *    - Collision Handling: Separate chaining (linked list of HashNodes).
 *
 * Time Complexity:
 * ----------------
 * - GET(key): O(1) average.
 *   (Hash lookup + Move node to Head)
 *
 * - PUT(key, value): O(1) average.
 *   (Hash lookup + Create/Update node + Move to Head + Potential Eviction)
 *
 * Memory Layout:
 * --------------
 * The visualization function in this file prints the actual memory addresses
 * to demonstrate how the Hash Table (Index) points to the Linked List (Data).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------------------------
// Data Structures
// ---------------------------------------------------------------------------

/**
 * Node (Doubly Linked List)
 * Stores the actual data (Key-Value pair) and links for ordering.
 */
typedef struct Node {
    int key;            // Stored to allow reverse lookup (Node -> Key) during eviction
    int value;          // The actual data
    struct Node *prev;  // Pointer to the previous node (towards MRU/Head)
    struct Node *next;  // Pointer to the next node (towards LRU/Tail)
} Node;

/**
 * HashNode (Hash Table Entry)
 * A wrapper used in the Hash Table's collision chains.
 * It points to the actual data Node in the Doubly Linked List.
 */
typedef struct HashNode {
    int key;                // Key for collision resolution
    Node *cacheNode;        // Pointer to the actual data Node in the DLL
    struct HashNode *next;  // Pointer to the next HashNode in this bucket (chaining)
} HashNode;

/**
 * LRUCache
 * The main container structure.
 */
typedef struct LRUCache {
    int capacity;           // Maximum number of items allowed
    int size;               // Current number of items
    Node *head;             // Pointer to the MRU node
    Node *tail;             // Pointer to the LRU node
    HashNode **hashTable;   // Array of pointers to HashNodes (the buckets)
    int hashSize;           // Size of the hash table array
} LRUCache;

// ---------------------------------------------------------------------------
// Helper Functions: Allocation & Hashing
// ---------------------------------------------------------------------------

/**
 * Creates a new Doubly Linked List Node.
 */
Node* createNode(int key, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        perror("Failed to allocate Node");
        exit(EXIT_FAILURE);
    }
    newNode->key = key;
    newNode->value = value;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

/**
 * Creates a new Hash Table Node.
 */
HashNode* createHashNode(int key, Node* cacheNode) {
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    if (!newNode) {
        perror("Failed to allocate HashNode");
        exit(EXIT_FAILURE);
    }
    newNode->key = key;
    newNode->cacheNode = cacheNode;
    newNode->next = NULL;
    return newNode;
}

/**
 * Initializes the LRU Cache.
 * @param capacity The maximum number of items the cache can hold.
 */
LRUCache* createCache(int capacity) {
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    if (!cache) {
        perror("Failed to allocate LRUCache");
        exit(EXIT_FAILURE);
    }
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    
    // We size the hash table larger than capacity (2x) to reduce collisions.
    // This is a simple heuristic for better performance.
    cache->hashSize = capacity * 2; 
    cache->hashTable = (HashNode**)calloc(cache->hashSize, sizeof(HashNode*));
    if (!cache->hashTable) {
        perror("Failed to allocate Hash Table");
        exit(EXIT_FAILURE);
    }
    
    return cache;
}

/**
 * Simple Hash Function.
 * Maps a key to a valid index in the hash table array.
 */
int hash(LRUCache* cache, int key) {
    return abs(key) % cache->hashSize;
}

// ---------------------------------------------------------------------------
// Internal Logic: Hash Table Management
// ---------------------------------------------------------------------------

/**
 * Adds a mapping (Key -> Node) to the Hash Table.
 * Handles collisions by adding to the front of the chain.
 */
void addToHash(LRUCache* cache, int key, Node* node) {
    int index = hash(cache, key);
    HashNode* newNode = createHashNode(key, node);
    
    // Insert at the beginning of the linked list for this bucket
    newNode->next = cache->hashTable[index];
    cache->hashTable[index] = newNode;
}

/**
 * Removes a mapping from the Hash Table.
 * Must traverse the collision chain to find the correct key.
 */
void removeFromHash(LRUCache* cache, int key) {
    int index = hash(cache, key);
    HashNode* current = cache->hashTable[index];
    HashNode* prev = NULL;

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

/**
 * Retrieves the DLL Node associated with a Key.
 * Returns NULL if the key is not found.
 */
Node* getFromHash(LRUCache* cache, int key) {
    int index = hash(cache, key);
    HashNode* current = cache->hashTable[index];
    
    while (current != NULL) {
        if (current->key == key) {
            return current->cacheNode;
        }
        current = current->next;
    }
    return NULL;
}

// ---------------------------------------------------------------------------
// Internal Logic: Doubly Linked List Management
// ---------------------------------------------------------------------------

/**
 * Adds a Node to the HEAD of the list (marking it as Most Recently Used).
 */
void addToHead(LRUCache* cache, Node* node) {
    node->next = cache->head;
    node->prev = NULL;
    
    if (cache->head != NULL) {
        cache->head->prev = node;
    }
    
    cache->head = node;
    
    // If the list was empty, this node is also the tail
    if (cache->tail == NULL) {
        cache->tail = node;
    }
}

/**
 * Unlinks a Node from its current position in the list.
 * Does NOT free the memory.
 */
void removeNode(LRUCache* cache, Node* node) {
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        // Node was Head
        cache->head = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        // Node was Tail
        cache->tail = node->prev;
    }
}

/**
 * Moves an existing Node to the HEAD (MRU position).
 */
void moveToHead(LRUCache* cache, Node* node) {
    removeNode(cache, node);
    addToHead(cache, node);
}

/**
 * Removes the Tail Node (Least Recently Used).
 * Returns the node pointer so the caller can free it and remove from hash.
 */
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

/**
 * GET Operation
 * Retrieves a value from the cache.
 * Side Effect: Moves the accessed item to the HEAD (MRU).
 * Returns -1 if not found.
 */
int get(LRUCache* cache, int key) {
    Node* node = getFromHash(cache, key);
    if (node == NULL) {
        return -1; // Not found
    }
    // Found: Update recency
    moveToHead(cache, node);
    return node->value;
}

/**
 * PUT Operation
 * Inserts a key-value pair into the cache.
 * If key exists: Updates value and moves to HEAD.
 * If key is new: Adds to HEAD. If full, evicts TAIL (LRU).
 */
void put(LRUCache* cache, int key, int value) {
    Node* node = getFromHash(cache, key);

    if (node != NULL) {
        // Case 1: Update existing key
        node->value = value;
        moveToHead(cache, node);
    } else {
        // Case 2: Insert new key
        Node* newNode = createNode(key, value);
        
        if (cache->size >= cache->capacity) {
            // Cache is full - Evict LRU
            Node* tail = removeTail(cache);
            removeFromHash(cache, tail->key);
            free(tail);
            cache->size--;
        }
        
        addToHead(cache, newNode);
        addToHash(cache, key, newNode);
        cache->size++;
    }
}

/**
 * Frees all memory associated with the cache.
 */
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
    
    // 3. Free Array and Struct
    free(cache->hashTable);
    free(cache);
}

// ---------------------------------------------------------------------------
// Visualization (Debug Tool)
// ---------------------------------------------------------------------------

/**
 * Prints a detailed ASCII diagram of the cache's internal memory state.
 * Shows pointers, addresses, and links between Hash Table and Linked List.
 */
void visualizeCache(LRUCache* cache) {
    printf("\n      Cache HEAD: %p  |  Cache TAIL: %p\n", (void*)cache->head, (void*)cache->tail);
    printf("      --------------------------------------------------------------------------------\n");
    printf("      [HASH TABLE]                [INTERMEDIARY]              [DOUBLY LINKED LIST]\n");
    printf("      (Array Idx)                 (HashNodes)                 (Data Nodes)\n\n");

    for (int i = 0; i < cache->hashSize; i++) {
        HashNode* hNode = cache->hashTable[i];
        
        if (hNode == NULL) {
            printf("   %d  [ NULL ]\n\n", i);
        } else {
            int first = 1;
            while (hNode != NULL) {
                // Determine if this node is Head or Tail for labeling
                const char* label = "";
                if (hNode->cacheNode == cache->head && hNode->cacheNode == cache->tail) label = " <--- HEAD & TAIL";
                else if (hNode->cacheNode == cache->head) label = " <--- HEAD";
                else if (hNode->cacheNode == cache->tail) label = " <--- TAIL";

                // Label for the HashNode box
                char hashTitle[20];
                if (first) sprintf(hashTitle, "HashHead");
                else       sprintf(hashTitle, "HashNode");

                // Print Hash Table connection or Chain connection
                if (first) {
                    printf("   %d  [ %14p ]------>[ %-10s ]              +--------------------------+\n", i, (void*)hNode, hashTitle);
                } else {
                    printf("                                   | next       \n");
                    printf("                                   v            \n");
                    printf("                                  [ %-10s ]              +--------------------------+\n", hashTitle);
                }
                
                // Print HashNode details and pointer to Data Node
                printf("                                  | @ %10p |              | @ %18p |%s\n", (void*)hNode, (void*)hNode->cacheNode, label);
                printf("                                  | Key: %-6d  |              | Key: %-6d Val: %-6d|\n", hNode->key, hNode->cacheNode->key, hNode->cacheNode->value);
                
                // Format pointers for Data Node
                char prevStr[20], nextStr[20];
                if (hNode->cacheNode->prev) sprintf(prevStr, "%p", (void*)hNode->cacheNode->prev); else sprintf(prevStr, "NULL");
                if (hNode->cacheNode->next) sprintf(nextStr, "%p", (void*)hNode->cacheNode->next); else sprintf(nextStr, "NULL");

                // Print Data Node links
                printf("                                  | cacheNode--|------------->| prev: %-18s |\n", prevStr);
                printf("                                  | %10p |              | next: %-18s |\n", (void*)hNode->cacheNode, nextStr);
                
                // Format next pointer for HashNode
                char nextHashStr[20];
                if (hNode->next) sprintf(nextHashStr, "%p", (void*)hNode->next); else sprintf(nextHashStr, "NULL");
                
                // Print HashNode footer
                printf("                                  | next-------|              +--------------------------+\n");
                printf("                                  | %10s | \n", nextHashStr);
                printf("                                  +------------+\n");
                
                hNode = hNode->next;
                first = 0;
            }
            printf("\n");
        }
    }
    
    // Print simple list order summary
    printf("      List Order (MRU -> LRU): ");
    Node* curr = cache->head;
    while(curr) {
        printf("[%d:%d] <-> ", curr->key, curr->value);
        curr = curr->next;
    }
    printf("NULL\n");
    printf("      --------------------------------------------------------------------------------\n");
}

// ---------------------------------------------------------------------------
// Main Driver (Demonstration)
// ---------------------------------------------------------------------------

int main() {
    // Setup: Capacity 5 -> Hash Size 10. Hash Function: Key % 10.
    printf("================================================================================\n");
    printf("LRU CACHE VISUALIZATION DEMO\n");
    printf("Capacity: 5 | Hash Size: 10 | Hash Function: Key %% 10\n");
    printf("================================================================================\n");

    LRUCache* cache = createCache(5);
    visualizeCache(cache);

    // Scenario 1: Basic Insertion & Chaining
    printf("\n[SCENARIO 1] Hash Collisions & Chaining\n");
    printf("Adding keys 1, 11, 21. All map to Index 1 (1 %% 10 == 1).\n");
    
    printf("\n> Put(1, 10)\n");
    put(cache, 1, 10);
    visualizeCache(cache);
    
    printf("\n> Put(11, 110)\n");
    put(cache, 11, 110);
    visualizeCache(cache);
    
    printf("\n> Put(21, 210)\n");
    put(cache, 21, 210);
    visualizeCache(cache);

    // Scenario 2: Filling the Cache
    printf("\n[SCENARIO 2] Filling the Cache\n");
    printf("Adding keys 2 and 12. Both map to Index 2.\n");
    
    printf("\n> Put(2, 20)\n");
    put(cache, 2, 20);
    visualizeCache(cache);
    
    printf("\n> Put(12, 120)\n");
    put(cache, 12, 120);
    visualizeCache(cache);
    
    // Scenario 3: Access & Reordering
    printf("\n[SCENARIO 3] Access & Reordering (MRU Update)\n");
    printf("Accessing Key 1. It is currently deep in the list and at the bottom of the hash chain.\n");
    printf("Expectation: Key 1 moves to HEAD of the list. Hash chain remains unchanged.\n");
    
    printf("\n> Get(1) -> Returns %d\n", get(cache, 1));
    visualizeCache(cache);

    // Scenario 4: Eviction
    printf("\n[SCENARIO 4] Eviction (Capacity Reached)\n");
    printf("Cache is full (Size 5). Adding Key 10 (Index 0).\n");
    printf("Expectation: LRU item (Key 11) is evicted. Key 10 is added.\n");
    
    printf("\n> Put(10, 100)\n");
    put(cache, 10, 100);
    visualizeCache(cache);

    // Scenario 5: Eviction with Collision
    printf("\n[SCENARIO 5] Eviction with Collision\n");
    printf("Adding Key 20 (Index 0). Collides with Key 10.\n");
    printf("Expectation: Next LRU item (Key 21) is evicted. Key 20 is added to Index 0 chain.\n");
    
    printf("\n> Put(20, 200)\n");
    put(cache, 20, 200);
    visualizeCache(cache);

    printf("\nCleaning up...\n");
    freeCache(cache);
    return 0;
}
