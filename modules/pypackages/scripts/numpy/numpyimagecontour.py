'''
/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/
'''
import inviwo #core inviwo module that retrieves properties etc.
import pypackagesutil #extended module that retrieves buffer data
import numpy as np
import matplotlib
matplotlib.use('Agg') #option to plot images and graphs in non-GUI mode
import matplotlib.pyplot as plt

#input image layer (SourceImage) which needs to be processed
SourceImage = pypackagesutil.getLayerData("NumpyImageContour", "SourceImage")
#iso/contour values
ContourValues = inviwo.getPropertyValue("NumpyImageContour","contourvalues")

###############################################################################
# convenience functions

# http://stackoverflow.com/questions/12201577/convert-rgb-image-to-grayscale-in-python
# http://en.wikipedia.org/wiki/Grayscale#Converting_color_to_grayscale
def rgb2gray(rgb):
   return np.dot(rgb[...,:3], [0.299, 0.587, 0.144])

#canvas out is retrieved as string and adapted
def copyFigureCanvasToImageBuffer(fig, InputImage):
   buf = np.fromstring ( fig.canvas.tostring_rgb(), dtype=InputImage.dtype )
   buf.shape = ( InputImage.shape[0], InputImage.shape[1], 3 )
   #roll and flip to fix orientation
   buf = np.roll ( buf, 3, axis = 2 )
   buf = np.flipud(buf)
   InputImage[:,:,0:3] = buf[:,:,:]

###############################################################################

#determine figure size using dpi
screenDim = (float(SourceImage.shape[0]), float(SourceImage.shape[1]))
screenDPI = float(plt.Figure()._get_dpi())
fsize=(screenDim[0]/screenDPI, screenDim[1]/screenDPI)
#set figure size
fig = plt.figure(figsize=fsize, dpi=screenDPI, frameon=False, linewidth=1.0)

#reshape input image for Numpy/C-Array consistency
newShape = (SourceImage.shape[1], SourceImage.shape[0], SourceImage.shape[2])
SourceImage.shape = newShape #reshaped 

#plot setting
#fit plotting to full window
plt.axis('off')
ax = plt.Axes(fig, [0., 0., 1., 1.])
ax.set_axis_off()
fig.add_axes(ax)

#plot input image
imgplot = plt.imshow(SourceImage, cmap=plt.cm.gray, origin='lower', interpolation='none')
#plot contour
plt.contour(rgb2gray(SourceImage), ContourValues)
plt.show()
fig.canvas.draw()

#copy result from matplot to SourceImage
copyFigureCanvasToImageBuffer(fig, SourceImage)
plt.close()