#ifndef BASICPANEL_HPP
#define BASICPANEL_HPP

#include <map>
#include "wx.hpp"
#include "configuration.hpp"

class BasicPanel : public wxPanel
{
        private:
                configuration &c;
                std::map<wxString, enzyme> &enzymes;
                wxListBox *fastaListBox;
                wxListBox *xmlListBox;
                wxCheckListBox *enzymeCheckListBox;
        public:
                BasicPanel(wxWindow *parent, wxWindowID id, configuration &c_, std::map<wxString, enzyme> &enzymes_);
                void update();
};

enum {
        ID_FastaListBox = wxID_HIGHEST + 1,
        ID_EnzymeCheckListBox = wxID_HIGHEST + 2
};

#endif
