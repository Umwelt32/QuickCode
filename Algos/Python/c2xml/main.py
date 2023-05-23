##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2023
##
## References:
##############
##############
##################################################
import sys,os
import py_c2xml

if __name__ == "__main__":
    if len(sys.argv)>1:
        fileName=str(sys.argv[1])
        print('Open File: '+fileName+'...')
        py_c2xml.generate_xml(fileName,fileName.replace('.','_')+'.xml')
        exit(0)
