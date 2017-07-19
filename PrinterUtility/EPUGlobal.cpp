#include "EPUGlobal.h"

EPUMainController* g_pMainController = NULL;

void SetMainController(EPUMainController *mainController)
{
    g_pMainController = mainController;
}

EPUMainController* GetMainController()
{
    return g_pMainController;
}

