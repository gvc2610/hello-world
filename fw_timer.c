/* Timer record for each timer */
typedef struct Timer
{
	unsigned short TimerId;
	unsigned short Period;
	struct Timer *pNext;
}tTimer;

static tTimer *mpTimerList = NULL;

/* Global variable for reference */
static unsigned short gTimer;



#define TIMER_1	            (unsigned short)0x0001
#define TIMER_2	            (unsigned short)0x0002
#define TIMER_3	            (unsigned short)0x0004
..........
#define TIMER_16	        (unsigned short)0x8000


bool AddTimer(unsigned short TimerId, unsigned short Period)
{
	tTimer *pTimer;
	tTimer *pNewTimer = NULL;
	bool ReturnValue = FALSE;

    /* Look for the timer – if already exists */
	pTimer = FindTimer(TimerId);

    /* Check if the timer was found */
	if((pTimer == NULL) || (pTimer->TimerId != TimerId))
    {
        /* Create a new timer */
    	pNewTimer = malloc(sizeof(tTimer));

    	if(pNewTimer != NULL)
        {
        	pNewTimer->TimerId = TimerId;
        	pNewTimer->pNext = NULL;

            /* Check if the list is empty */
        	if(pTimer == NULL)
            {
                /* Store the address of this timer as a first element in the list */
            	mpTimerList = pNewTimer;
            }
        	else
            {
                /* Add the new timer to the end of the list */
            	pTimer->pNext = pNewTimer;
            }
        }

        /* Select the new timer */
    	pTimer = pNewTimer;
    }

	if(pTimer != NULL)
    {
        /* Set the timer interval */
    	pTimer->Period = Period;
    	ReturnValue = TRUE;
    }

	return ReturnValue;
}


static tTimer * FindTimer(unsigned short TimerId)
{
	tTimer *pTimer;

    /* Move to the start of the list */
	pTimer = mpTimerList;

    /* Check if list is empty */
	if(pTimer != NULL)
    {
        /* Move through the list until the timer id is found */
    	while((pTimer->Next != NULL) && (pTimer->TimerId != TimerId))
        {
            /* Timer ids - not match, Move onto the next timer */
        	pTimer = pTimer->Next;
        }
    }
	return (pTimer);
}


#pragma interrupt_level 0
void interrupt TimerBase(void)
{
    /* Check the bit for a timer 0 interrupt */
	if(TMRFLAG == 1)
    {
        /* Reset corresponding bit */
    	TMRFLAG = 0;

        /* Reload the corresponding register value */
    	RELOAD_TIMER0();

        /* Process timer event */
    	SystemTickEvent();

        /* Enable the timer0 again */
        ......
    }
}

#pragma inline SystemTickEvent
void SystemTickEvent(void)
{
	tTimer *pTimer;

    /* Update the timers */
	pTimer = mpTimerList;

	while(pTimer != NULL)
    {
    	if(pTimer->Period != 0)
        {
            /* Decrement the timer period */
        	pTimer->Period--;

            /* Check if the timer has reached zero */
        	if(pTimer->Period == 0)
            {
                /* Set the corresponding bit when the timer reaches zero */
            	gTimer = gTimer | pTimer->TimerID;
            }
        }
        /* Move to the next timer in the list */
    	pTimer = pTimer->pNext;
    }
}


void CheckTimerEvent()
{
	undigned short nTimer;

    /* Read the global variable gTimer and reset the value */
	DisableInterrupt()
	nTimer = gTimer; 
	gTimer = 0;
	EnableInterrupt()

    /* Check for TimerId */
	if( nTimer != 0)
    {
    	if(nTimer & TIMER_1)
        {
            /* Call Timer 1 routine */
        	Timer1Function();
        }
    	if(TimerID & TIMER_2)
        {
            /* Call Timer 2 routine */
        	Timer2Function();
        }
        ........
    }
}

void main(void)
{
    /* Initialize all registers & variables*/
	InitMain ();
    ........
    
    /* Initial value of gTimer = 0 */
	gTimer = 0;

    /* Add a Timer1 with 1000 ms interval*/
	AddTimer(TIMER_1, 1000);

    /* Add a Timer2 with 500 ms interval*/
	AddTimer(TIMER_1, 500);

    ........
	while(1)
    {
        /* Check the Timer Events */
    	CheckTimerEvent();
        .......
        /* Write other task here */
    }
}



