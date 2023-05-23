##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2023
##
## References:
##############
##############
##################################################
import os,sys,numpy,re
import xml.etree.ElementTree as ETR

def generate(in_xml,out_xml):
    _generate(in_xml,out_xml)

def _generate(in_xml,out_xml):
    xml_reader = ETR.parse(in_xml)
    xml_writer = ETR.Element('root')
    xml_writer_current = xml_writer
    xml_writer_parent  = []
    xml_writer_last_block_id = ['','']
    for element in xml_reader.getroot().iter():
        TAG  = str(element.tag)
        TEXT = str(element.text)
        if TAG=='function':
            xml_writer_parent = []
            xml_writer_current = xml_writer
            xml_writer_parent.append(xml_writer_current)
            xml_writer_current.append(ETR.Element('function'))
            xml_writer_current = xml_writer_current[-1]
            xml_writer_current.attrib['name'] = TEXT
            xml_writer_last_block_id = ['function',TEXT]
        elif TAG=='conditional':
            xml_writer_last_block_id = [str(element.attrib['type']),TEXT]
        elif TAG=='block':
            if TEXT=='{':
                xml_writer_parent.append(xml_writer_current)
                xml_writer_current.append(ETR.Element('block'))
                xml_writer_current = xml_writer_current[-1]
                xml_writer_current.attrib['type']    = xml_writer_last_block_id[0]
                xml_writer_current.attrib['content'] = xml_writer_last_block_id[1]
                xml_writer_last_block_id = ['','']
            elif TEXT=='}':
                xml_writer_last_block_id = ['','']
                xml_writer_current = xml_writer_parent.pop()
            else:
                print('BLOCK_ERROR')
        elif TAG=='action':
            xml_writer_current.append(ETR.Element('action'))
            xml_writer_current[-1].attrib['type']    = str(element.attrib['type'])
            xml_writer_current[-1].attrib['function']= str(element.attrib['function'])
            xml_writer_current[-1].text=TEXT
    _save_txt_file(str(ETR.tostring(xml_writer).decode('ascii')),out_xml)

def _save_txt_file(data,path):
    with open(path, "w") as f: f.write(data)
