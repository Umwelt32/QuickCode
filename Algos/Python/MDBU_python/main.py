##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## Convert canny detector samples
## References:
##############
##############
##################################################
import cv2,sys,os,subprocess
import canny_out
import hgt2bmp

def run_mdbu_sw(seed,h,i,file_in,file_out):
    params = ['ds1.exe','-h',str(h),'-i',str(i),'-seed',str(seed),'-file',str(file_in),'-out',str(file_out)]
    subprocess.run(params)
    
def parse_file(fileName,out):
    f1 = out+fileName.replace('.','_')
    f1_in = f1+'.bmp'
    f1_in_canny = f1+'_canny.bmp'
    f1_in_mdbu = f1+'_mdbu.bmp'
    hgt2bmp.convHgtToBmp(fileName,f1_in)
    canny_out.canny_output(f1_in,f1_in_canny)
    run_mdbu_sw(128,0.3,-1,f1_in_canny,f1_in_mdbu)

def convHgtToBmpRecurse(in_path):
     list = [f for f in os.listdir(in_path) if f.endswith('.hgt')]
     for file in list:
        parse_file(file,'./out/')

if __name__ == "__main__":
    if len(sys.argv)>1:
        fileName=str(sys.argv[1])
        print('Open File: '+fileName+'...')
        parse_file(fileName,'./out/')
        exit(0)
    else:
        print("Missing input argument! - path to .hgt file.")
        cwd = os.getcwd()
        convHgtToBmpRecurse(cwd)
        exit(0xAA)

