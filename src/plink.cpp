#include <cassert>
#include <stdio.h>
#include <windows.h>

static int StrLen(const char *s)
{
    int result = 0;
    while (*s)
    {
        result++;
        s++;
    }
    return result;
}

static HDDEDATA CALLBACK DdeCallback(UINT /*type*/,
                                     UINT /*fmt*/,
                                     HCONV /*hconv*/,
                                     HSZ /*hsz1*/,
                                     HSZ /*hsz2*/,
                                     HDDEDATA /*hdata*/,
                                     ULONG_PTR /*data1*/,
                                     ULONG_PTR /*data2*/)
{
    return nullptr;
}

bool DDEExecute(char *server, char *topic, char *command)
{
    DWORD inst = 0;
    HSZ hszServer = nullptr, hszTopic = nullptr;
    HCONV hconv = nullptr;
    bool ok = false;
    unsigned int result = 0;
    DWORD cbLen = 0;
    HDDEDATA answer;

    printf("%d", StrLen(command));
    assert(StrLen(command) < INT_MAX - 1);
    if (StrLen(command) >= INT_MAX - 1)
    {
        return false;
    }

    result = DdeInitializeW(&inst, DdeCallback, APPCMD_CLIENTONLY, 0);
    if (result != DMLERR_NO_ERROR)
    {
        return false;
    }

    hszServer = DdeCreateStringHandle(inst, server, CP_WINNEUTRAL);
    if (!hszServer)
    {
        goto Exit;
    }
    hszTopic = DdeCreateStringHandle(inst, topic, CP_WINNEUTRAL);
    if (!hszTopic)
    {
        goto Exit;
    }
    hconv = DdeConnect(inst, hszServer, hszTopic, nullptr);
    if (!hconv)
    {
        goto Exit;
    }

    cbLen = ((DWORD)StrLen(command) + 1) * sizeof(WCHAR);
    answer = DdeClientTransaction((BYTE *)command,
                                  cbLen,
                                  hconv,
                                  nullptr,
                                  CF_UNICODETEXT,
                                  XTYP_EXECUTE,
                                  10000,
                                  nullptr);
    if (answer)
    {
        DdeFreeDataHandle(answer);
        ok = true;
    }

Exit:
    if (hconv)
    {
        DdeDisconnect(hconv);
    }
    if (hszTopic)
    {
        DdeFreeStringHandle(inst, hszTopic);
    }
    if (hszServer)
    {
        DdeFreeStringHandle(inst, hszServer);
    }
    DdeUninitialize(inst);

    return ok;
}

bool DDERetrieve(char *server, char *topic, char *command)
{
    DWORD inst = 0;
    HSZ hszServer = nullptr, hszTopic = nullptr;
    HCONV hconv = nullptr;
    bool ok = false;
    unsigned int result = 0;
    DWORD cbLen = 0;
    HDDEDATA answer;

    printf("%d", StrLen(command));
    assert(StrLen(command) < INT_MAX - 1);
    if (StrLen(command) >= INT_MAX - 1)
    {
        return false;
    }

    result = DdeInitialize(&inst, DdeCallback, APPCMD_CLIENTONLY, 0);
    if (result != DMLERR_NO_ERROR)
    {
        return false;
    }

    hszServer = DdeCreateStringHandle(inst, server, CP_WINNEUTRAL);
    if (!hszServer)
    {
        goto Exit;
    }
    hszTopic = DdeCreateStringHandle(inst, topic, CP_WINNEUTRAL);
    if (!hszTopic)
    {
        goto Exit;
    }

    HSZ hszCommand;
    hszCommand = DdeCreateStringHandle(inst, command, CP_WINNEUTRAL);
    if (!hszTopic)
    {
        goto Exit;
    }

    hconv = DdeConnect(inst, hszServer, hszTopic, nullptr);
    if (!hconv)
    {
        goto Exit;
    }

    cbLen = ((DWORD)StrLen(command) + 1) * sizeof(WCHAR);
    answer = DdeClientTransaction(NULL,
                                  0,
                                  hconv,
                                  hszCommand,
                                  CF_TEXT,
                                  XTYP_REQUEST,
                                  10000,
                                  nullptr);
    if (answer)
    {
        BYTE *data;
        DWORD dataSize;

        data = DdeAccessData(answer,   // [in]            HDDEDATA hData,
                             &dataSize // [out, optional] LPDWORD  pcbDataSize
        );
        // TODO: pull the data and return it

        DdeFreeDataHandle(answer);
        ok = true;
    }
    else{
        int result;
        result = DdeGetLastError(inst);
        int omg = result;
    }

Exit:
    if (hconv)
    {
        DdeDisconnect(hconv);
    }
    if (hszTopic)
    {
        DdeFreeStringHandle(inst, hszTopic);
    }
    if (hszServer)
    {
        DdeFreeStringHandle(inst, hszServer);
    }
    DdeUninitialize(inst);

    return ok;
}

int main()
{
    // char *command =
    //     "[Open(\"C:\\Users\\bogda\\Downloads\\01_2IRR00_Setting_Up_The_Tools.pdf\",0,1,0)]";
    // printf("%s", command);

    // DDEExecute("SUMATRA", "control", command);

    DDERetrieve("SUMATRA" , "control" , "[GetFileState()]");
}
