#!/usr/bin/env python
# 
# the following code is from 
# Converting CSV to TSV from StackExchange 
# cricket_007
#
import csv, sys
csv.writer(sys.stdout, dialect='excel-tab').writerows(csv.reader(sys.stdin))
