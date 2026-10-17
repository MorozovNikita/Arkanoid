#include "BlockFactory.h"

#include "DurableBlockFactory.h"
#include "GlassBlockFactory.h"
#include "SmoothDestroyableBlockFactory.h"
#include "UnbreakableBlockFactory.h"

#include <array>

namespace Game
{
    int BlockFactory::GetCreatedBreackableBlocksCount()
    {
        return createdBreackableBlocksCount;
    }

    void BlockFactory::ClearCounter()
    {
        createdBreackableBlocksCount = 0;
    }

    namespace
    {
        std::array<BlockFactory*, 4>& factories()
        {
            static SmoothDestroyableBlockFactory smooth;
            static DurableBlockFactory durable;
            static GlassBlockFactory glass;
            static UnbreakableBlockFactory unbreakable;
            static std::array<BlockFactory*, 4> all = {
                &smooth,
                &durable,
                &glass,
                &unbreakable
            };
            return all;
        }
    }

    BlockFactory* findBlockFactory(std::string_view type)
    {
        for (BlockFactory* factory : factories())
        {
            if (factory->type() == type)
                return factory;
        }
        return nullptr;
    }

    void clearBlockFactoryCounters()
    {
        for (BlockFactory* factory : factories())
            factory->ClearCounter();
    }
}
