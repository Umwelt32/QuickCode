import pefile

m_debug_module = None
m_symbols_list = None

def _load_symbols_list(path):
    global m_debug_module
    global m_symbols_list
    try:
        m_debug_module = pefile.PE(str(path))
        m_symbols_list = [[export.name.decode('ascii'),hex(export.address)] for export in m_debug_module.DIRECTORY_ENTRY_EXPORT.symbols]
    except Exception as e:
        print(str(e))
        return False
    else:
        return True

if __name__ == "__main__":
    _load_symbols_list('ddraw.dll')
    print(str(m_symbols_list))
