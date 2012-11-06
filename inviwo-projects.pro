TEMPLATE = subdirs

include(config.txt)

SUBDIRS += sub_ticpp
SUBDIRS += sub_core
SUBDIRS += sub_qtwidgets
SUBDIRS += sub_editor
#SUBDIRS += sub_glutminimum
SUBDIRS += sub_inviwo

sub_ticpp.file = ext/ticpp/ticpp.pro

sub_core.file = src/core/inviwo-core.pro
sub_core.depends = sub_ticpp

sub_qtwidgets.file = src/qt/widgets/inviwo-qtwidgets.pro
sub_qtwidgets.depends = sub_ticpp sub_core

sub_editor.file = src/qt/editor/inviwo-editor.pro
sub_editor.depends = sub_ticpp sub_core sub_qtwidgets

#sub_glutminimum.file = apps/glutminimum/glutminimum.pro
#sub_glutminimum.depends = sub_ticpp sub_core

sub_inviwo.file = apps/inviwo/inviwo.pro
sub_inviwo.depends = sub_ticpp sub_core sub_qtwidgets sub_editor

### Local Variables:
### mode:conf-unix
### End: