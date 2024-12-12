// Exception handling globals
static FUNCPTR test_HELIX_104320_excBaseHookBk;
static TASK_ID test_HELIX_104320_TaskId;
static int test_HELIX_104320_excHandledFlag = 0;
extern FUNCPTR _func_excBaseHook;
void* test_HELIX_104320_excRetAddr = NULL;

// Exception handler
static BOOL test_HELIX_104320_excHook
    (
    int             vecNum,
    void        *   pEsf,
    REG_SET     *   pRegSet,
    EXC_INFO    *   pExcInfo
    )
    {
    if (taskIdSelf() == test_HELIX_104320_TaskId)
        {
        test_HELIX_104320_excHandledFlag = 1;
        pRegSet->pc = (INSTR *) (test_HELIX_104320_excRetAddr);

        return TRUE;
        }

    if (test_HELIX_104320_excBaseHookBk != NULL)
        {
        return test_HELIX_104320_excBaseHookBk (vecNum, pEsf, pRegSet, pExcInfo);
        }

    return FALSE;
    }

// Exception hook setup/cleanup
static void test_HELIX_104320_setExcHook(void* retAddr)
    {
    if (test_HELIX_104320_excBaseHookBk == NULL)
        {
        test_HELIX_104320_excBaseHookBk = _func_excBaseHook;
        }
    _func_excBaseHook = test_HELIX_104320_excHook;
    test_HELIX_104320_excRetAddr = retAddr;    
    }

static void test_HELIX_104320_reSetExcHook()
    {
    _func_excBaseHook = test_HELIX_104320_excBaseHookBk;
    }

void test_isalnum_HELIX_104320(void (*setup)(void), void (*cleanup)(void)) 
{
    (*setup)();
    
    // Initialize exception handling
    test_HELIX_104320_TaskId = taskIdSelf ();
    test_HELIX_104320_excHandledFlag = 0;

    // Set exception hook before critical section
    test_HELIX_104320_setExcHook(&&test_HELIX_104320_excLabel);

    // TEST IMPLEMENTATION
    // Critical section that may cause exception
    int result;
    int c = INT_MAX; // Test input

    // Execute test
    result = isalnum(c);

    // Verify results
    // Since the behavior is undefined for INT_MAX, we expect an exception

    test_HELIX_104320_excLabel:
        test_HELIX_104320_reSetExcHook();
        
    printf ("HELIX-104320: %s",
            test_HELIX_104320_excHandledFlag ? 
                    "Exception successfully handled\n" 
                    : "Exception not handled\n");    

    // Cleanup
    (*cleanup)();
}

void setup(void) {
    // Resource initialization
    // Error handling
}

void cleanup(void) {
    // Resource cleanup
    // Error handling
}