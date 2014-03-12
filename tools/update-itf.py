#!/usr/bin/env python3

#requires python3, lxml, and beautifulsoup4

import sys
import os
from bs4 import BeautifulSoup

for f in sys.argv[1:]:
    print("Open file " + f)

    with open(f, 'r') as file:
        filestr = "\n".join(file.readlines())

    soup = BeautifulSoup(filestr, 'xml')
	
    treedata = soup.find_all("InviwoTreeData")

    for tf in treedata:
		
        if "reference" in tf.attrs:
            continue

        print("BEFORE " + 60*"#")
        print(tf.prettify())
        print("AFTER  " + 60*"#")

        np = int(tf.size["content"])

        tf.size.decompose()
    
        newtf = soup.new_tag("InviwoTreeData")

        newtf.append(soup.new_tag("maskMin", content=tf.mask_["x"]))
        newtf.append(soup.new_tag("maskMax", content=tf.mask_["y"]))

        tf.mask_.decompose()

        dps = soup.new_tag("dataPoints")
        for i in range(0,np):
            p = tf.find("pos"+str(i)).extract()
            c = tf.find("rgba"+str(i)).extract()
            p.name = "pos"
            c.name = "rgba"
        
            newp = soup.new_tag("point")
        
            newp.append(p)
            newp.append(c)
        
            dps.append(newp)

        newtf.append(dps)
        
        ipt = tf.interpolationType_.extract()
        ipt.name = "interpolationType_"
        newtf.append(ipt)

        tf.decompose()
		
        soup.append(newtf)
        print(newtf.prettify())
        print("DONE   " + 60*"#")

        
    print("Write file ")

    with open(f, 'w') as fout:
        fout.write(soup.prettify())


