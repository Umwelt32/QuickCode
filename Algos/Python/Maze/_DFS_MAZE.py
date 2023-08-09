##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## Convert DFS algorithm
## References:
##############
##############
##################################################

import cv2,os,sys,numpy,math

m_maze = None
m_cell_stack = None

def _init(n,r,seed):
    global m_maze
    global m_cell_stack
    numpy.random.seed(seed)
    m_maze = numpy.zeros((n, n), dtype=numpy.uint8)
    m_cell_stack = []
    _put_rand_visitor(n,r)

def _put_rand_visitor(n,r):
    for x in range(r): _setVisited(_rnd_rand_int(0,n-1),_rnd_rand_int(0,n-1))

def _rnd_rand_int(s,e):
    if (s==e): return s
    return int(numpy.random.randint(s,e))

def _setVisited(x,y):
    global m_maze
    global m_cell_stack
    if m_maze[x][y]==0:
        m_cell_stack.append([x,y])
        m_maze[x][y]=1

def _getCellValue(x,y):
    global m_maze
    value = None
    try:
        value = m_maze[x][y]
    except:
        value = None
    return value

def _getUnvisitedNList(x,y):
    global m_maze
    global m_cell_stack
    nb_coord_list_ex = [[x-1,y-1],[x-1,y],[x-1,y+1],[x+1,y-1],[x+1,y+1],[x+1,y],[x,y+1],[x,y-1]]
    nb_coord_list = [[x-1,y],[x+1,y],[x,y-1],[x,y+1]]
    list = [_x for _x in nb_coord_list if _getCellValue(_x[0],_x[1])==0]
    return list

def _processUnvisitedNB(x,y):
    list = _getUnvisitedNList(x,y)
    if (len(list)>0):
        vs = list[_rnd_rand_int(0,len(list)-1)]
        _setVisited(vs[0],vs[1])

def _processStack():
    global m_maze
    global m_cell_stack
    if len(m_cell_stack)>0:
        element = m_cell_stack.pop()
        _processUnvisitedNB(element[0],element[1])

def _process(n):
    global m_cell_stack
    if n==0:
        while(len(m_cell_stack)>0):
            _processStack()
    else:
        for x in range(n):_processStack()
        
def _save_im(filename):
    global m_maze
    m_maze[m_maze==1]=255
    cv2.imwrite(filename, m_maze)

if __name__ == "__main__":
    _init(64,8,128)
    _process(0)
    _save_im('out.bmp')

    