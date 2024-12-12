// Include necessary headers
#include <vxWorks.h>
#include <stdio.h>
#include <ctype.h>
#include <errnoLib.h>

// Exception handling globals
static FUNCPTR test_isalnum_helix_75966_excBaseHookBk;
static TASK_ID test_isalnum_helix_75966_TaskId;
static int test_isalnum_helix_75966_excHandledFlag = 0;
extern FUNCPTR _func_excBaseHook;
void* test_isalnum_helix_75966_excRetAddr = NULL;

// Exception handler
static BOOL test_isalnum_helix_75966_excHook
    (
    int             vecNum,
    void        *   pEsf,
    REG_SET     *   pRegSet,
    EXC_INFO    *   pExcInfo
    )
    {
    if (taskIdSelf() == test_isalnum_helix_75966_TaskId)
        {
        test_isalnum_helix_75966_excHandledFlag = 1;
        pRegSet->pc = (INSTR *) (test_isalnum_helix_75966_excRetAddr);

        return TRUE;
        }

    if (test_isalnum_helix_75966_excBaseHookBk != NULL)
        {
        return test_isalnum_helix_75966_excBaseHookBk (vecNum, pEsf, pRegSet, pExcInfo);
        }

    return FALSE;
    }

// Exception hook setup/cleanup
static void test_isalnum_helix_75966_setExcHook(void* retAddr)
    {
    if (test_isalnum_helix_75966_excBaseHookBk == NULL)
        {
        test_isalnum_helix_75966_excBaseHookBk = _func_excBaseHook;
        }
    _func_excBaseHook = test_isalnum_helix_75966_excHook;
    test_isalnum_helix_75966_excRetAddr = retAddr;    
    }

static void test_isalnum_helix_75966_reSetExcHook()
    {
    _func_excBaseHook = test_isalnum_helix_75966_excBaseHookBk;
    }

void test_isalnum_helix_75966(void (*setup)(void), void (*cleanup)(void)) 
{
    (*setup)();
    
    // Initialize exception handling
    test_isalnum_helix_75966_TaskId = taskIdSelf ();
    test_isalnum_helix_75966_excHandledFlag = 0;

    // Set exception hook before critical section
    test_isalnum_helix_75966_setExcHook(&&test_isalnum_helix_75966_excLabel);

    // TEST IMPLEMENTATION
    // Critical section that may cause exception
    // 1. Set up test data
    int testChars[] = {'A', 'a', '0', '!', 'Z', 'z', '9', '@'};
    int expectedResults[] = {_UP, _LO, _DI, 0, _UP, _LO, _DI, 0};
    int numTests = sizeof(testChars) / sizeof(testChars[0]);
    int result;
    int i;

    // 2. Execute test
    for (i = 0; i < numTests; i++) {
        result = isalnum(testChars[i]);

        // 3. Verify results
        if (result != expectedResults[i]) {
            printf("FAILED: isalnum('%c') returned %d, expected %d\n", testChars[i], result, expectedResults[i]);
        } else {
            printf("PASSED: isalnum('%c') returned %d as expected\n", testChars[i], result);
        }
    }

    // 4. Check error conditions using errnoGet() if applicable
    if (errnoGet() != 0) {
        printf("FAILED: errnoGet() returned non-zero error code\n");
    }

    test_isalnum_helix_75966_excLabel:
        test_isalnum_helix_75966_reSetExcHook();
        
    printf ("HELIX-75966: %s",
            test_isalnum_helix_75966_excHandledFlag ? 
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