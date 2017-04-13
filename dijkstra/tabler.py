import sys, os, csv, argparse;
import math;
from operator import itemgetter

#Prepare the arguments the program shall receive
def __prepareargs__():
    parser = argparse.ArgumentParser(description='Creates result tables for N-Heap Dijkstra implementation')
    parser.add_argument('-d', nargs=1, type=str, help='directory with results', required=True)
    parser.add_argument('-o', nargs=1, type=str, help='output file name', required=True)
    parser.add_argument('-x', nargs=1, type=int, help='experiment identifier (insert, update, delete=1, comp1=2, comp2=3, scale=4)', required=True)
    parser.add_argument('-g', nargs=1, type=str, help='output file for graphic plotting', required=False, default = None)
    parser.add_argument('-e', nargs=1, type=str, help='result files extension (default = \'.dat\')', required=False, default = ['.dat'])

        
    return parser

#Parse the input arguments and returns a dictionary with them
def __getargs__(parser):
    args = vars(parser.parse_args())
    return args

def __proc_exp1__(dirname, outfname, rfext):
    
    outf = open(outfname, "w")
    print("reading files") 
    for file in [f for f in os.listdir(dirname) if f.endswith(rfext)]:
        lr = []
        print("Reading file "+file)
        with open(dirname+file, 'rb') as csvfile:
            csvdata = csv.reader(csvfile, delimiter=' ')

            for row in csvdata:
                i = int(row[0])
                nswaps = int(row[1])
                time = float(row[3])
                tpe = float(row[4])  
                
                l = [i, nswaps, time, tpe]           
                lr.append(l)

	
	    lr = sorted(lr, key=itemgetter(0))
	    outf.write("----"+dirname+file+"----\n")    
        for l in lr:
            #outf.write("%i %Le\n" % (l[0], l[3]))
            outf.write("%i & %i & %.2Le\n" % (l[0], l[1], l[2]))
        outf.write("------------------------\n")



def __proc_exp23__(dirname, outfname, rfext):
    lr = []
    outf = open(outfname, "w")
    for file in [f for f in os.listdir(dirname) if f.endswith(rfext)]:
        with open(dirname+file, 'rb') as csvfile:
            csvdata = csv.reader(csvfile, delimiter=',')
            fl = csvdata.next()
            n = int(fl[1])
            m = int(fl[2])
            nins = int(fl[3])
            ndel = int(fl[4])
            nupd = int(fl[5])
            mem  = int(fl[6])
            time = float(fl[7])
            
            nexp = 1
            for row in csvdata:
                nins += int(row[3])
                ndel += int(row[4])
                nupd += int(row[5])
                mem  += int(row[6])
                time += float(row[7])
                nexp += 1
           
            nins_avg = float(nins/nexp)
            ndel_avg = float(ndel/nexp)
            nupd_avg = float(nupd/nexp)
            mem_avg  = float(mem/nexp)
            time_avg = float(time/nexp)
           
            l = [n, m, nins_avg, ndel_avg, nupd_avg, mem_avg, time_avg]
            lr.append(l)     


    lr = sorted(lr, key=itemgetter(0))    
    outf = open(outfname, "w")
    
    for l in lr:
        outf.write("%i\t%i\t%f\t%f\t%f\t%f\t%Le\n" % (l[0], l[1], l[2], l[3], l[4], (l[5]/(1024*1024)), l[6]))

   
def __proc_exp11__(dirname, outfname, rfext):
    lr = []
    outf = open(outfname, "w")
    for file in [f for f in os.listdir(dirname) if f.endswith(rfext)]:
        print "reading "+file
        with open(dirname+file, 'rb') as csvfile:
            csvdata = csv.reader(csvfile, delimiter=' ')
            fl = csvdata.next()
            k = int(fl[0])
            ns = 0
            e = 0
            t  = 0
            idt = 0
            
            nexp = 1
            for row in csvdata:
                idt = int(row[0])
                ns = int(row[1])
                e = int(row[2])
                t  = float(row[3])
                l = [k, idt, ns, e, t]                
                lr.append(l) 
           
            #print ("appending %i %i %i\n" % (k, n, m))
                

    
    lr = sorted(lr, key = lambda x: (x[1], x[0]))    
    outf = open(outfname, "w")
    nn = -1
    for l in lr:
        if(l[1] != nn):
            nn = l[1]
            outf.write("\n%i & " % (l[1]))
            
        outf.write(" %i & %i & %.2Le &" % (l[3], l[2], l[4]))


def __proc_exp44__(dirname, outfname, rfext):
    lr = []
    outf = open(outfname, "w")
    for file in [f for f in os.listdir(dirname) if f.endswith(rfext)]:
        with open(dirname+file, 'rb') as csvfile:
            csvdata = csv.reader(csvfile, delimiter='\t')
            fl = csvdata.next()
            k = int(fl[0])
            n = 0
            m = 0
            mem  = 0
            time = 0
            
            nexp = 1
            for row in csvdata:
                n = int(row[0])
                m = int(row[1])
                mem  = float(row[5])
                time = float(row[6])
                l = [k, n, m, mem, time]                
                lr.append(l) 
           
            #print ("appending %i %i %i\n" % (k, n, m))
                

    
    lr = sorted(lr, key = lambda x: (x[1], x[0]))    
    outf = open(outfname, "w")
    nn = 0
    for l in lr:
        if(l[1] != nn):
            nn = l[1]
            outf.write("\n%i & %i & %.2f &" % (l[1], l[2], l[3]))
        outf.write(" %.2Le &" % (l[4]))
   

def __proc_exp55__(dirname, outfname, rfext):

    outf = open(outfname, "w")
    for file in [f for f in os.listdir(dirname) if f.endswith(rfext)]:
        lr = []
        with open(dirname+file, 'rb') as csvfile:
            csvdata = csv.reader(csvfile, delimiter='\t')
            print "reading "+file+"\n"
            for row in csvdata:
                n = int(row[0])
                m = int(row[1])
                I  = float(row[2])
                D = float(row[3])
                U = float(row[4])
                t = float(row[6])
                
                if I > n:
                    print "Error on I for "+file
                if D > n:
                    print "Error on D for "+file
                if U > m:
                    print "Error on U for "+file
                
                tp = (t/((m*math.log(n))+(n*math.log(n))))
                print file+"\n"
                print ("t = %.2Le | tp = %.2Le | (m+n)log(n) = %.2Le | log(%d) = %.2Le\n" % (t, tp, (m*math.log(n))+(n*math.log(n)), n, math.log(n)))
                
                l = [n, m, tp, t]                
                lr.append(l) 
                   
            lr = sorted(lr, key = lambda x: (x[0]))    
            
            outf.write(file+"\n")    
            for l in lr:
                outf.write("%i %i %.2Le %.2Le\n" % (l[0], l[1], l[2], l[3]))

            outf.write("--------------------------------\n")
            
            for l in lr:
                outf.write("%i & %i & %.2Le\n" % (l[0], l[1], l[3]))
                
            outf.write("================================\n")
            




#The main module, in case this program was called as the main program
if __name__ == '__main__':
    #receive and prepare the arguments
    parser = __prepareargs__()
    args = __getargs__(parser)
    
    #read parameters
    dirname  = args['d'][0]
    outfname = args['o'][0]
    exp      = int(args['x'][0])
    
    gfname = None
    if(args['g'] != None):
        gfname   = args['g'][0]
    rfext    = args['e'][0]
    
    if exp == 1:
        __proc_exp1__(dirname, outfname, rfext)    
    elif exp == 2 or exp == 3:
        __proc_exp23__(dirname, outfname, rfext)
    elif exp == 4:
        __proc_exp44__(dirname, outfname, rfext)
    elif exp == 5:
        __proc_exp55__(dirname, outfname, rfext)
    elif exp == 11:
        __proc_exp11__(dirname, outfname, rfext)

    
   
    

    
    
                
                
