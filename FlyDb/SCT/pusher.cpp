#include <Logger.hpp>
#include <FlyDbClient.hpp>

bool checkRv(int pRv)
{
    if (FlyDbClient::TIMEOUT == pRv)
    {
        Logless("ERROR: TIMEOUT!");
        return false;
    }
    else if (FlyDbClient::BADRESP == pRv)
    {
        Logless("ERROR: BADRESP!");
        return false;
    }
    else if (FlyDbClient::OK == pRv)
    {
        Logless("OK!");
        return true;
    }
    Logless("ERROR: UNSPECIFIED!");
    return false;
}
int main(int argc, const char *argv[])
{
    Logger::getInstance().logful();

    FlyDbClient flyDb(net::toIpPort(127,0,0,1,9000),100000);
    uint32_t i = 0;
    while(true)
    {
        uint8_t id = i%16;
        Logless("SET GROUP [_] to [_]", (unsigned)id, id+2);
        auto rv = flyDb.set(ParamIdDataArray{
            ParamIdData{ParamId(id), int32_t(id++)},
            ParamIdData{ParamId(id), int16_t(id++)},
            ParamIdData{ParamId(id), int8_t(id++)},
            ParamIdData{ParamId(id), int8_t(id++)}
        });

        if (checkRv(rv))
        {
            id = i%16;
            Logless("GET GROUP [_] to [_]", (unsigned)id, id+2);
            ParamDataArray result;
            rv = flyDb.get(ParamIdArray{id++,id++,id++,id++}, result);
            id = i%16;
            if(checkRv(rv))
            {
                if (4 != result.size())
                {
                    Logless("ERROR: Invalid result size!");
                }

                if (std::get<int32_t>(result[0]) != int32_t(id++)) Logless("ERROR: Invalid valid result[0]=_!", std::get<int32_t>(result[0]));
                if (std::get<int16_t>(result[1]) != int16_t(id++)) Logless("ERROR: Invalid valid result[1]=_!", std::get<int16_t>(result[1]));
                if (std::get<int8_t> (result[2]) != int8_t(id++))  Logless("ERROR: Invalid valid result[2]=_!", (unsigned)std::get<int8_t>(result[2]));
                if (std::get<int8_t> (result[3]) != int8_t(id++))  Logless("ERROR: Invalid valid result[2]=_!", (unsigned)std::get<int8_t>(result[3]));
            }
        }
        i+=4;;
    }
}
