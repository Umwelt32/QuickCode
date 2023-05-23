##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2023
##
## References:
##############
##############
##################################################
import os,sys,numpy,re

m_current_function_data = None
m_current_for = None
m_current_function = None
m_current_if = None
m_new_data = None

def generate_xml(in_txt,out_xml):
    global m_function_data
    global m_new_data
    m_function_data = _load_txt_file(in_txt)
    _add_xml_tags(m_function_data)
    _save_txt_file(out_xml,m_new_data)
    
def _load_txt_file(path):
    f = open(path, "r")
    data = [x for x in f]
    f.close();
    return data

def _save_txt_file(path,data):
    f = open(path, "w")
    for element in data:
        f.write(element)
    f.close()

def _add_xml_tags(current_functions_list):
    global m_current_function_data
    global m_current_actions
    global m_current_function
    global m_current_if
    global m_current_for
    global m_current_while
    global m_current_else
    global m_current_elseif
    global m_new_data
    m_new_data = ['<?xml version="1.0"?>\n<root>\n']
    for data in current_functions_list:
        m_current_function_data = data
        m_current_function_data = _replace_strings(m_current_function_data,['&','<','>','"',"'"],['&amp ','&lt ','&gt ','&quot ','&apos '])
        m_current_function_data = m_current_function_data.replace("else if","else_if")
        m_current_function= re.findall('[\w\s]+[^{};]*\s*',             m_current_function_data)
        m_current_actions = re.findall('[\w\s]+[^{};]*\s*;',            m_current_function_data)
        m_current_if      = re.findall('\s*^if\s*\(+[^)]*\)+[^{;]',     m_current_function_data)
        m_current_for     = re.findall('\s*for\s*\(+[^)]*\)+[^{;]',     m_current_function_data)
        m_current_while   = re.findall('\s*while\s*\(+[^)]*\)+[^{;]',   m_current_function_data)
        m_current_elseif  = re.findall('\s*else_if\s*\(+[^)]*\)+[^{;]', m_current_function_data)
        _replace_tags()
        m_new_data.append('<entry>'+m_current_function_data.replace('\n',' ')+'</entry>\n')
    m_new_data.append('</root>\n')

def _replace_tags():
    global m_current_function_data
    global m_current_actions
    global m_current_function
    global m_current_if
    global m_current_for
    global m_current_while
    for f in m_current_function[0:1]: m_current_function_data = m_current_function_data.replace(f,'<function>'+f.strip()+'</function>')
    for f in m_current_if: m_current_function_data = m_current_function_data.replace(f,'<conditional type="if">'+f.strip()+'</conditional>')
    for f in m_current_for: m_current_function_data = m_current_function_data.replace(f,'<conditional type="for">'+f.strip()+'</conditional>')
    for f in m_current_while: m_current_function_data = m_current_function_data.replace(f,'<conditional type="while">'+f.strip()+'</conditional>')
    for f in m_current_elseif: m_current_function_data = m_current_function_data.replace(f,'<conditional type="else_if">'+f.strip()+'</conditional>')
    for f in m_current_actions:
        type = 'call' if ');' in f else 'action'
        function_list = re.findall('[\w]+\([^)]*\)', f)
        function      = function_list[0] if len(function_list) else ''
        function_list = re.findall('[\w]+\(', function)
        function      = function_list[0] if len(function_list) else ''
        function      = function.replace('(','').strip()
        m_current_function_data = m_current_function_data.replace(f,'<action type="'+type+'" function="'+function+'">'+f.strip()+'</action>')
    m_current_function_data = m_current_function_data.replace('else ','<conditional type="else">else</conditional>')
    m_current_function_data = _replace_strings(m_current_function_data,['}','{'],['<block>}</block>','<block>{</block>'])
    m_current_function_data = _replace_strings(m_current_function_data,['&amp ','&lt ','&gt ','&quot ','&apos '],['&amp;','&lt;','&gt;','&quot;','&apos;'])
    
def _replace_strings(input,pattern,new_pattern):
    for idx in range(len(pattern)):input=input.replace(pattern[idx],new_pattern[idx])
    return input