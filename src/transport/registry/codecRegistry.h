#pragma once

#include <memory>
#include <map>
#include <vector>

#include "../codec/abstractCodec.h"

namespace Quix {

    class CodecRegistry{

        private:
            CodecRegistry(){};
            std::map<std::string, std::vector<AbstractCodec*>> codecs;

        public:
            CodecRegistry(CodecRegistry const&) = delete;
            CodecRegistry& operator=(CodecRegistry const&) = delete;
            static std::shared_ptr<CodecRegistry> instance()
            {
                static std::shared_ptr<CodecRegistry> s{new CodecRegistry};
                return s;
            }

            void registerCodec(const std::string& model, AbstractCodec*);
            std::vector<AbstractCodec*>& retrieveCodecs(const std::string& modelKey);
            void clearCodecs(const std::string& modelKey);
            AbstractCodec* retrieveCodec(const std::string& modelKey, const std::string& codecId);

        
    };


}