/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Andreas Valter
 *
 **********************************************************************/

#include <inviwo/core/util/commandlineparser.h>

namespace inviwo {

CommandLineParser::CommandLineParser(){
    cmd_ = new TCLAP::CmdLine("Inviwo description...", ' ', "0.0.0");
}

CommandLineParser::CommandLineParser(int argc, char **argv) : argc_(argc), argv_(argv) 
{
    cmd_ = new TCLAP::CmdLine("Inviwo description...", ' ', "0.0.0");
}

CommandLineParser::~CommandLineParser() {
    delete cmd_;
    delete workspaceValueArg_;
    delete outputValueArg_;
    delete snapshotArg_;
    delete screenGrabArg_;
    delete pythonScriptArg_;
    delete noSplashScreenArg_;
    delete quitArg_;
}

void CommandLineParser::initialize() {
    // Set up available arguments and flags
    try{  
        workspaceValueArg_ = new TCLAP::ValueArg<std::string>("w",
                                                            "workspacePath",
                                                            "Specify workspace to open",
                                                            false,
                                                            "",
                                                            "Name of workspace");
        outputValueArg_ = new TCLAP::ValueArg<std::string>("o",
            "outputPath",
            "Specify output path",
            false,
            "",
            "Output path");
        pythonScriptArg_ = new TCLAP::ValueArg<std::string>("p",
                                                            "pythonScript",
                                                            "Specify a python script to run at startup",
                                                            false,
                                                            "",
                                                            "Path to the file containing the script");
		snapshotArg_ = new TCLAP::ValueArg<std::string>("s",
			"snapshot",
			"Specify default name of each snapshot, or empty string for processor name.",
			false,
			"",
			"Snapshot default name: UPN=Use Processor name.");

		screenGrabArg_ = new TCLAP::ValueArg<std::string>("g",
			"screengrab",
			"Specify default name of each screengrab.",
			false,
			"",
			"");

		quitArg_ = new TCLAP::SwitchArg("q", "quit", 
			"Pass this flag if you want to close inviwo after startup.");
		noSplashScreenArg_ = new TCLAP::SwitchArg("n", "nosplash", 
			"Pass this flag if you do not want to show a splash screen.");
        
        cmd_->add(*workspaceValueArg_);
        cmd_->add(*pythonScriptArg_);
        cmd_->add(*outputValueArg_);
		cmd_->add(*snapshotArg_);
		cmd_->add(*screenGrabArg_);
		cmd_->add(*quitArg_);
		cmd_->add(*noSplashScreenArg_);
    } catch (TCLAP::ArgException &e) {
		LogError(e.error() << " for arg " << e.argId());
	}
}

const std::string CommandLineParser::getOutputPath() const{
    if (outputValueArg_->isSet()){
        return (outputValueArg_->getValue());
    }
    return "";
}

const std::string CommandLineParser::getWorkspacePath() const{
    if (workspaceValueArg_->isSet()){
        return (workspaceValueArg_->getValue());
    }
    return "";
}

void CommandLineParser::parse(int argc, char** argv){
    try{        
        cmd_->parse(argc, argv);
    } catch (TCLAP::ArgException &e) { 
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; // catch exceptions 
    }
}

void CommandLineParser::parse(){
    parse(argc_, argv_);
}

bool CommandLineParser::getCaptureAfterStartup() const{
	return snapshotArg_->isSet();
}

const std::string CommandLineParser::getSnapshotName() const{
	if (snapshotArg_->isSet()){
		return (snapshotArg_->getValue());
	}
	return "";
}


bool CommandLineParser::getScreenGrabAfterStartup() const{
	return screenGrabArg_->isSet();
}

const std::string CommandLineParser::getScreenGrabName() const{
	if (screenGrabArg_->isSet()){
		return (screenGrabArg_->getValue());
	}
	return "";
}


bool CommandLineParser::getRunPythonScriptAfterStartup() const{
    return pythonScriptArg_->isSet();
}

const std::string CommandLineParser::getPythonScirptName() const{
    if(pythonScriptArg_->isSet()){
        return (pythonScriptArg_->getValue());
    }
    return "";
        
}

bool CommandLineParser::getQuitApplicationAfterStartup() const{
	return quitArg_->getValue();
}

bool CommandLineParser::getShowSplashScreen() const{
	return !(noSplashScreenArg_->isSet());
}

bool CommandLineParser::getLoadWorkspaceFromArg() const{
    if (workspaceValueArg_->isSet()){
        std::string values = workspaceValueArg_->getValue();

        assert(values.size() != 0);
        return true;
    }
    return false;
}

void CommandLineParser::deinitialize() {
}

} // namespace
