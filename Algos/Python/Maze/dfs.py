import cv2,os,sys,numpy,math

m_maze = None

def _init(n):
    global m_maze
    m_maze = numpy.zeros((n*2, n*2), dtype=numpy.uint8)
    for x in range(0,m_maze.shape[0],2):
        for y in range(0,m_maze.shape[0],2):
            m_maze[x][y]=2

def _dfs(x,y):
    global m_maze
    xy   = [[x+1,y],[x-1,y],[x,y+1],[x,y-1]]
    xy_z = [[x+2,y],[x-2,y],[x,y+2],[x,y-2]]
    if _setCellValue(x,y,1)==False: return False
    for idx in range(len(xy_z)):
        if _getCellValue(xy_z[idx][0],xy_z[idx][1])==2:
            _setCellValue(xy[idx][0],xy[idx][1],1)
            _dfs(xy_z[idx][0],xy_z[idx][1])
    
def _save_img(filename):
    global m_maze
    m_maze[m_maze==1]=255
    m_maze[m_maze==2]=128
    cv2.imwrite(filename, m_maze)

def _getCellValue(x,y):
    global m_maze
    try:
        value = m_maze[x][y]
    except:
        value = None
    return value

def _setCellValue(x,y,v):
    global m_maze
    try:
        m_maze[x][y]=v
    except:
        return False
    else:
        return True

if __name__ == "__main__":
    _init(32)
    _dfs(16,8)
    _save_img('out.bmp')
    