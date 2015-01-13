#!/usr/bin/env python
# coding=utf-8
#/*********************************************************************************
# *
# * Inviwo - Interactive Visualization Workshop
# * Version 0.6b
# *
# * Copyright (c) 2013-2015 Inviwo Foundation
# * All rights reserved.
# *
# * Redistribution and use in source and binary forms, with or without
# * modification, are permitted provided that the following conditions are met:
# *
# * 1. Redistributions of source code must retain the above copyright notice, this
# * list of conditions and the following disclaimer.
# * 2. Redistributions in binary form must reproduce the above copyright notice,
# * this list of conditions and the following disclaimer in the documentation
# * and/or other materials provided with the distribution.
# *
# * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# *
# * Main file author: Daniel Jönsson
# *
# *********************************************************************************/

# Script to calculate convergence rate and 
# generate plots for convergence test of volume renderer
# based on article Verifying Volume Rendering Using Discretization Error Analysis
# http://scivis.itn.liu.se/publications/2014/EJRSCNKYS14/

from optparse import OptionParser
import sys
import math
import struct
import random
import os
#from PIL import Image
#from PIL import ImageChops
#from PIL import ImageOps
#from PIL import ImageStat
#from PIL import ImageEnhance

from pylab import *
import matplotlib as mpl 
from matplotlib.font_manager import fontManager, FontProperties
import pylab
from pylab import arange,pi,sin,cos,sqrt
import numpy, scipy 
from scipy import ndimage
from itertools import product

def main():
    usage = "%prog [OPTIONS] image"
    parser = OptionParser(usage, version="%prog (Convergence rate of images)")
    parser.add_option("-p", "--plot", action="store_true", \
               dest="plot", default=False, \
               help = "Plot convergence results")
    parser.add_option("-o", "--output", action="store",    \
               dest="output", type="string", default="", \
               help = "SVG output file")
    parser.add_option("--show", action="store_true",    \
               dest="show", default=False, \
               help = "Shows the plot")

    (options, args) = parser.parse_args(sys.argv)
    if len(args) < 2:
        parser.print_help()
        return 0

    plotResult = options.plot
    output     = options.output
    showfig    = options.show
    refinement_factor = 2.0

    print "Starting execution"
    filenames = args[1:]


    images = []
    for filename in filenames:
        try:
            image = plt.imread(filename)
            #image = Image.open(filename)
            images.append(image)
        except IOError:
            print "\t\tFile", filename, "could not be loaded."

    if len(images) < 3:
        print "\tCould not load enough images (", len(images),\
            "images were loaded ). Aborting..."
        return
    else:
        print "\t",len(images), "images were loaded"  

    maxSize = -1
    for im in images: 
        maxSize = max(im.size, maxSize)
    # Divide by four since we have four channels
    # print maxSize/4
    if maxSize != images[0].size:
        new_images = [] 
        newSize = (int(sqrt(maxSize/4)), int(sqrt(maxSize/4)))
        print "\tUpsampling to size", int(sqrt(maxSize/4)),"^2"
        for im in images:
            # Calculate ratio, assumes square images
            ratio = int(sqrt(maxSize/4))/int(sqrt(im.size/4))
            # Resample using nearest neighbour
            new_images.append(ndimage.zoom(im, (ratio, ratio, 1), order=0))
            #misc.imsave('resampledImage%d.tif' % ratio, immm)
        images = new_images

    imagesError = []
    for i in xrange(len(images)-1):
        imagesError.append(fabs(images[i]-images[i+1]))
        #imagesError.append(ImageChops.difference(images[i], images[i+1]))
    y = []
    for i in xrange(len(imagesError)):
        E_curr = imagesError[i].max()
        #E_curr = max(imagesError[i].getextrema())
        #print E_curr
        y.append(math.fabs(E_curr+0.000000000001))

    print "Errors:", y 

    # Plotting
    if plotResult == True:
        try:
            markers =     ['-k', '-.ob', ':sc', '-.^g', '-vm', '--Dr', '--Dy']
            markersface = [ 'w', 'b',    'c',    'g',   'w',    'w',   'y']
            markersedge = [ 'w', 'b',    'c',    'g',   'm',    'r',   'y']

            scale = refinement_factor

            x = [];
            d = 0.25
            for i in xrange(len(y)):
                d /= scale
                x.append(d)

            
            
            # Linear regression of points in 
            # logarithmic scale using x and y vector.
            xp  = log(x)
            yp  = log(y);
            m, b = polyfit(xp, yp, 1)


            x_first_order = []
            y_first_order = []
            y_zero_order = []
            vx = scale * x[0]
            vy = scale * y[0]
            for i in xrange(len(x)+2):
                x_first_order.append(vx)
                y_first_order.append(vy)
                y_zero_order.append(y[0])
                vx /= scale
                vy /= scale
            
            if output == "stepsize_convergence" or output == "pixel_convergence":
                # First order convergence
                methods = [['Expected, 1st order slope', y_first_order, x_first_order]]
            elif output == "dataset_convergence":
                # Zero order convergence
                methods = [['Expected, 0 order slope', y_zero_order, x_first_order]]
            else:
                methods = [['1st order slope', y_first_order, x_first_order]]
                methods += [['0 order slope', y_zero_order, x_first_order]]
                
            # Numerical method
            methods += [[output + ' ($k = %2.2f$)' % m, y, x]]


            # Plotting
            fig = gcf()
            fig.set_facecolor('white')

            #print "\tCreating plots..."

            legends = []
            k = 0
            for i in methods:
                plot = loglog(i[2], i[1], markers[k], linewidth=1.5,           \
               markerfacecolor=markersface[k], markeredgecolor=markersedge[k], \
                       markersize=8, antialiased = True, basex=10)    
                k += 1
                legends += [i[0]]    

            axes = plot[0].get_axes()

            mpl.rcParams['grid.color'] = '1.0'

            # Setting graph parameters
            params = {'xtick.labelsize' : 20,\
                          'ytick.labelsize' : 20,\
                          'grid.color' : (0.5, 0.5, 0.5, 1.0)
                      }
            pylab.rcParams.update(params)

            # Plotting settings
            grid(True)
            xlabel(r"$\log(N)$", size=25)
            ylabel('$\log(L_\infty$ norm) ', size=25)

            font= FontProperties(size=15);
            legend(legends, loc='lower right', prop=font)

            if output != "":
                savefig(output + ".svg", facecolor='w', edgecolor='w', orientation='portrait',  format='svg', transparent=False, bbox_inches=None, pad_inches=0.1)

            if showfig == True:
                show()

        except e:
            print "Could not find pylab. Figure can't be rendered."

            return




if __name__ == "__main__":
    main()
