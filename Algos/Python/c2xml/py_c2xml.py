##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2023
##
## References:
##############
##############
##################################################
import os,sys,numpy,re
import py_c2xml_gen
import py_postXmlGen

m_c_functions_regex_def = ''
m_c_file_data = None
m_c_functions_list = []
m_c_functions_data = None

def generate_xml(c_path,out_path):
    global m_c_file_data
    global m_c_functions_data
    txt_path = str(out_path).replace('.','_')+'.txt'
    gen_xml_path = txt_path.replace('.txt','_temp.xml')
    m_c_file_data = _load_c_file(c_path)
    m_c_functions_data = _parse_c_file_list()
    _save_c_file(txt_path,m_c_functions_data)
    py_c2xml_gen.generate_xml(txt_path,gen_xml_path)
    py_postXmlGen.generate(gen_xml_path,out_path)
    
def _load_c_file(path):
    f = open(path, "r")
    lines = [line for line in f]
    f.close()
    data = ' '.join(lines).replace('\n',' ').replace('  ',' ')
    return data

def _save_c_file(path,data):
    f = open(path, "w")
    for element in data:
        f.write(element+'\n')
    f.close()

def _parse_c_file_list():
    global m_c_file_data
    global m_c_functions_regex_def
    list1    = [x.strip() for x in re.findall('[\w\s]+\([^)]*\)\s*{', m_c_file_data) if not any(elem in x for elem in ['while','for','if','else','case','switch'])]
    function_bodies = []
    for x in list1:
        start = m_c_file_data.find(x)
        start_l = start+len(x)
        for y in range(int(len(m_c_file_data)-start)):
            if m_c_file_data[start:start_l+y].count('{')==m_c_file_data[start:start_l+y].count('}'):
                data = m_c_file_data[start:start_l+y]
                data = data.replace('\t',' ').replace('\n',' ')
                data = re.sub(r'\s+', ' ', data)
                data = re.sub(r'\/\*.*?\*\/', ' ', data)
                data = data.strip()
                function_bodies.append(data)
                break
    return function_bodies
