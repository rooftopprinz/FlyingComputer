#ifndef CONFIG_TREE_PARSER_HPP
#define CONFIG_TREE_PARSER_HPP
/******************************************************************************
*       Copyright Nokia 2019
*
*       The copyright notice above does not evidence any
*       actual or intended publication of such source code.
*       The code contains Nokia Confidential Proprietary Information.
*
******************************************************************************/
#include "ConfigTree.hpp"
#include "rapidxml/rapidxml.hpp"

#include <algorithm>
#include <list>
#include <numeric>
#include <string>
#include <vector>

struct ConfigParser
{
    ConfigParser(const std::string& xmlData)
        : root({})
    {
        using rapidxml::xml_document;
        using rapidxml::xml_node;

        xml_document<> doc;
        xml_node<>*    root_node;

        doc.parse<0>(const_cast<char*>(xmlData.c_str())); // TODO: dangerous make sure parse flag is 0
        root_node = doc.first_node("config");
        if (!root_node)
        {
            throw std::runtime_error("<config> not found");
        }
        auto                     currentNode    = root_node->first_node();
        auto                     currentCfgNode = &root;
        std::vector<xml_node<>*> retNodes;
        std::vector<ConfigTree*> retCfgNodes;
        std::vector<std::string> path;

        while (true)
        {
            if (currentNode)
            {
                auto        indexAttr = currentNode->first_attribute("index");
                auto        xrefAttr  = currentNode->first_attribute("xref");
                int         index     = indexAttr ? std::stoi(indexAttr->value()) : -1;
                const char* name      = currentNode->name();
                const char* value     = currentNode->value();

                std::string nameId = name;
                if (xrefAttr)
                {
                    nameId += "Ref";
                }

                nameId += index >= 0 ? "-" + std::to_string(index) : "";

                std::string pathStr = std::accumulate(
                    path.begin(), path.end(), std::string(), [](std::string& a, const std::string& b) { return a + b + std::string("/"); });
                pathStr += nameId;

                auto node = currentNode->first_node();

                auto child = currentCfgNode->createChild(nameId.c_str());

                if (!child)
                {
                    throw std::runtime_error(std::string() + "Child can't be added: " + nameId);
                }
                if (!currentNode->value_size() && node)
                {
                    retNodes.emplace_back(currentNode);
                    retCfgNodes.emplace_back(currentCfgNode);
                    path.emplace_back(nameId);
                    currentCfgNode = child;
                    currentNode    = node;
                }
                else
                {
                    if (currentNode->value_size())
                    {
                        auto type = getFtype(value);
                        if (Ftype::INTEGER == type)
                        {
                            child->setData(static_cast<unsigned long long>((unsigned long long)std::stoull(value)));
                        }
                        else if (Ftype::FLOATING == type)
                        {
                            child->setData(static_cast<long double>(std::stold(value)));
                        }
                        else
                        {
                            child->setData(std::string(value));
                        }
                    }
                    if (xrefAttr && xrefAttr->value_size())
                    {
                        unsigned long long refId = std::stoull(xrefAttr->value());
                        child->setData(refId);
                    }
                    else if (xrefAttr)
                    {
                        throw std::runtime_error(std::string() + "xref can't be left blank.");
                    }
                    currentNode = currentNode->next_sibling();
                }
            }
            else
            {
                if (!retNodes.size()) break;
                currentNode = retNodes.back();
                auto name   = path.back();
                retNodes.pop_back();
                path.pop_back();
                currentNode    = currentNode->next_sibling();
                currentCfgNode = retCfgNodes.back();
                retCfgNodes.pop_back();
            }
        }
    }

    enum Ftype
    {
        INTEGER,
        FLOATING,
        STRING
    };

    Ftype getFtype(const char* value)
    {
        size_t len     = std::strlen(value);
        size_t c       = 0;
        size_t d       = 0;
        bool allNumber = std::all_of(value, value + len, [&c, &d](const char i) {
            c++;
            if (std::isdigit(i) || (i == '-' && c == 1)) return true;
            if (i == '.' && !d)
            {
                d++;
                return true;
            }
            return false;
        });

        if (allNumber && d)
        {
            return Ftype::FLOATING;
        }
        else if (allNumber)
        {
            return Ftype::INTEGER;
        }
        return Ftype::STRING;
    }

    ConfigTree root;
};

#endif // CONFIG_TREE_PARSER_HPP
