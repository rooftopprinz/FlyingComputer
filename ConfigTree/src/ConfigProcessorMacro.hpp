#define CONFIG_BEGIN(type)                          \
    \
template<>                                          \
        \
struct CfgProcessor<type> : public CfgProcessorBase \
    {                                               \
        template <typename Doer>                    \
        void process(const char* n, type* v)        \
        {                                           \
            Doer doit(n, cursor, size, cfgTree);    \
            doit
#define FIELD(name) << std::make_pair(#name, &v->name)
#define CONFIG_END(name) \
    ;                    \
    }                    \
    }                    \
    ;
