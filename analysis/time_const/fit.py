import ROOT as rt

cv = rt.TCanvas("cv","histogram",0,200,700,500)
g = rt.TGraphErrors("result2.dat")
#g = rt.TGraph("result.dat")
g.SetTitle("Decay Constant(Am241 B side);Time[sec];Amplitude")
#g.GetXaxis().SetTitle("Time[sec]")
#g.GetYaxis().SetTitle("Amplitude")
g.GetXaxis().CenterTitle()
g.GetYaxis().CenterTitle()

"""
f1 = rt.TF1("f1","[0]*exp(-(x-[1])/[2])+[3]",3e-6,2e-3)
f1.SetParameters(-0.0904461,5.56839e-05,1.36384e-05,-0.513675)
f1.SetLineColor(2)
f2 = rt.TF1("f2","[0]*exp(-(x-[1])/[2])+[3]",3e-6,10e-6)
f2.SetParameters(-2.36542e-10,9.78887e-05,4.05502e-06,-2.2643)
f2.SetLineColor(3)
f = rt.TF1("f","[0]*exp(-(x-[1])/[2])+[3]",1e-6,3e-6)
f.SetParameters(-1.71149e-43,3.6164e-05,3.49995e-07,-6.23983)
f.SetLineColor(4)
#f2 = rt.TF1("f2","[0]*exp(-(x-[1])/[2])+[3]*exp(-(x-[1])/[4])+[5]",0.24e-6,4e-6)
#f2.SetParameters(-2.27941e-10,3.54249e-5,3.46722e-06,1.90283e-06,3.46722e-06,-0.2)
#f2.SetLineColor(3)
"""
"""
f1 = rt.TF1("f1","[0]*exp(-(x-[1])/[2])+[3]*exp(-(x-[1])/[4])+[5]",1e-6,0.2e-3)
f1.SetParameters(-0.488059,3.45736e-05,1.5107e-05,-1.03705e-13,1.05624e-06,-0.502653)
#f1.SetParLimits(4,1.0e-06,1.0e-05)
f1.SetLineColor(2)
f2 = rt.TF1("f2","[0]*exp(-(x-[1])/[2])+[3]",0.24e-6,0.32e-6)
f2.SetParameters(-4.89907e-43,3.52515e-05,3.48415e-07,15.136)
f2.SetLineColor(3)
"""

f1 = rt.TF1("f1","[0]*exp(-(x-[1])/[2])+[3]*exp(-(x-[1])/[4])+[5]*exp(-(x-[1])/[6])+[7]",1.0e-6,2e-4)
f1.SetParameters(-2.23028e-02,1.06800e-04,2.21270e-05,-1.44603e-08,5.49740e-06,-3.92031e-53,8.65809e-07,-4.66559e-01)
#f1.SetParameters(-0.110924,6.17784e-05,1.90696e-05,-3.55116e-07,3.86152e-06,-6.81079e-53,5.013e-07,-0.290618)
#f1.SetParameters(-0.120679,3.00423e-05,1.30907e-05,-1.28262e-14,9.1917e-07,-4.89907e-43,3.48415e-07,0.231059)
#f1.SetParLimits(4,1.0e-06,1.0e-05)
f1.SetLineColor(2)


g.Draw()
g.Fit("f1","R")
g.Fit("f2","R+")
g.Fit("f","R+")
chi2 = f1.GetChisquare()
NDf = f1.GetNDF()
print(chi2)
print(NDf)

cv.Update()
rt.TPython.Prompt()

#cv.Print("graph1.png")
