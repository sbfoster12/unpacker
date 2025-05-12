import os 

'''
    Python script to loop through the runs processed by the nearline and hadd subruns together if
    1) the hadded file doesn't exist
    or 
    2) a subrun file has a modification date later than the hadded files creation data 
'''

indir = "/home/pioneer/pioneer/pioneer-teststand/nearline/"
outdir = "/home/pioneer/pioneer/pioneer-teststand/nearline_hadd/"
# outdir_histsOnly = '/datalocal/nearlineArts/nearlineHists_merged_noTrees/'

# get a list of the subrun files
subruns = os.listdir(indir)

#parse file names into run numbers: nearline_hists_run00278_00044.root 
runs = []
for si in subruns:
    # print(si)
    try:
        runi = int(si.split("_run")[1].split("_")[0]) 
        runs.append( runi )
        # print("    ",runi)
    except:
        print("     Unable to parse file:", si)
        continue
runs = list(set(runs))
# print("Checking runs:", runs[:10],"...")

# loop over all runs and check if there is an hadded file
for i, run in enumerate(runs):
    do_hadd_file = False
    print("*******************************************************************************")
    print("Processing run:", run, "(", i+1,"/",len(runs),")")
    #does the hadded file exist
    hadd_name = outdir+"nearline_hists_run"+str(run)+".root"
    # hadd_name_histsOnly = outdir_histsOnly+"nearline_hists_run"+str(run)+".root"
    these_subruns = [x for x in subruns if str(run) in x]
    if(os.path.exists(hadd_name)):
        #now check if any new subrun files are created
        hadd_time = os.path.getmtime(hadd_name)
        print("This run hadded at:", hadd_time)
        print("Now checking:", these_subruns)
        for si in these_subruns:
            thistime = os.path.getmtime(indir+si)
            print('       This time:', thistime, thistime > hadd_time)
            if(thistime > hadd_time):
                print("New subrun detected!")
                do_hadd_file = True
                break
        # pass
    else:
        #file does not exist, definitely hadd
        print("File not found, hadding...")
        do_hadd_file = True


    if(do_hadd_file):
        #remove the existing hadd file and create a new one
        print("hadding file...")
        os.system("rm -f "+hadd_name) #remove existing file
        hadd_command = "time hadd -f "+hadd_name+" "
        for si in these_subruns:
            hadd_command += " "+indir+si+" "
        # print(hadd_command)
        os.system(hadd_command)

        #create version without trees
        # os.system("rm -f "+hadd_name_histsOnly) #remove existing file
        # hadd_command_histsOnly = 'hadd -f -T '+hadd_name_histsOnly+' '+hadd_name

    # break
