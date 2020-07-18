#!/usr/bin/python

import re
import sys
with open (sys.argv[1], 'r' ) as f:
    content = f.read()
content_new = re.sub('struct (.*)\n{\n([\n A-Za-z0-9_;<>:/]+?)\n};', r'CONFIG_BEGIN(\1)\n\2\nCONFIG_END(\1)', content)
content_new = re.sub('    .* (.*);', r'    FIELD(\1)', content_new)
content_new = re.sub('[\n]+', r'\n', content_new)

content_new = \
    '#pragma once\n' + \
    '#include "config.hpp"\n' + \
    '#include "ConfigProcessor.hpp"\n' + \
    '#include "ConfigProcessorMacro.hpp"\n' + \
    content_new + "\n" + \
    '#undef CONFIG_BEGIN\n' + \
    '#undef FIELD\n' + \
    '#undef CONFIG_END\n'

print content_new