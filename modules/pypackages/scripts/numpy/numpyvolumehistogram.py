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

#input volume
SourceVolume = pypackagesutil.getVolumeData("NumpyVolumeHistogram", "SourceVolume")
#output histogram image
OutputImage = pypackagesutil.getLayerData("NumpyVolumeHistogram", "OutputImage")
#save pdf
savePDF = pypackagesutil.getBufferData("NumpyVolumeHistogram","SavePDF") #TODO: Instead of buffers use regular bool
#save pdf dir
pdfDir = inviwo.getPropertyValue("NumpyVolumeHistogram","pdfDir")

###############################################################################
# convenience functions

#canvas out is retrieved as string and adapted
def copyFigureCanvasToImageBuffer(fig, InputImage):
   buf = np.fromstring ( fig.canvas.tostring_rgb(), dtype=InputImage.dtype )
   buf.shape = ( InputImage.shape[0], InputImage.shape[1], 3 )
   #roll and flip to fix orientation
   buf = np.roll ( buf, 3, axis = 2 )
   buf = np.flipud(buf)
   InputImage[:,:,0:3] = buf[:,:,:]

def saveFigureCanvasToPDF(fig, PDFDir, PDFPrefix='pdfImage'):
   from matplotlib.backends.backend_pdf import PdfPages
   import time
   PDFfileName = PDFDir + '/' + PDFPrefix + '_' + time.strftime("%H-%M-%S") + time.strftime("-%d-%m-%Y") + ".pdf"
   print PDFfileName
   pp = PdfPages(PDFfileName)
   pp.savefig(fig)
   pp.close()

###############################################################################

#determine figure size using dpi
screenDim = (float(OutputImage.shape[0]), float(OutputImage.shape[1]))
screenDPI = float(plt.Figure()._get_dpi())
fsize=(screenDim[0]/screenDPI, screenDim[1]/screenDPI)
#set figure size
fig = plt.figure(figsize=fsize, dpi=screenDPI, frameon=False, linewidth=1.0)

#reshape input image for Numpy/C-Array consistency
newShape = (OutputImage.shape[1], OutputImage.shape[0], OutputImage.shape[2])
OutputImage.shape = newShape #reshaped 

#plot setting
#fit plotting to full window
plt.axis('off')
ax = plt.Axes(fig, [0., 0., 1., 1.])
ax.set_axis_off()
fig.add_axes(ax)

#plot histogram
plt.hist(SourceVolume.flatten(), 256, range=(0,256), facecolor='green', alpha=0.75)
plt.xlabel('Voxel Intensity')
plt.ylabel('Voxel Count')
plt.title('Volume Histogram')
plt.grid(True)

plt.show()
fig.canvas.draw()

#save pdf
if savePDF[0]==1:
   saveFigureCanvasToPDF(fig, pdfDir, 'numpyVolumeHistogram')
   savePDF[0] = 0

#copy result from matplot to OutputImage
copyFigureCanvasToImageBuffer(fig, OutputImage)
OutputImage[:,:,3] = 255 #alpha
plt.close()
