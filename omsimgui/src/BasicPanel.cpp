#include "BasicPanel.hpp"
#include "ListBoxPanel.hpp"
#include "EnzymeDialog.hpp"

#include <algorithm> //find

BasicPanel::BasicPanel(wxWindow *parent, wxWindowID id, configuration &c_, std::map<wxString, enzyme> &enzymes_)
      : wxPanel(parent, id), c(c_), enzymes(enzymes_)
{
        wxBoxSizer *mainbox = new wxBoxSizer(wxVERTICAL);
        
        /*
                name box
        */
        wxBoxSizer *namebox = new wxBoxSizer(wxVERTICAL);
        wxStaticText *nameTitle = new wxStaticText(this, wxID_ANY, wxT("Experiment name"));
        nameCtrl = new wxTextCtrl(this, wxID_Name, c.get("name"), wxPoint(-1, -1), wxSize(-1, -1));
        namebox->Add(nameTitle);
        namebox->Add(nameCtrl, 0, wxEXPAND | wxALL, 20);
        
        Connect(wxID_Name, wxEVT_TEXT,
                wxCommandEventHandler(BasicPanel::OnName));
        
        /*
                fasta box
        */
        wxBoxSizer *fastabox = new wxBoxSizer(wxVERTICAL);
        wxStaticText *fastaTitle = new wxStaticText(this, wxID_ANY, wxT("Genome files"));
        
        wxPanel * fastaPanel = new wxPanel(this, -1);
        wxBoxSizer *flbbox = new wxBoxSizer(wxHORIZONTAL);
        fastaListBox = new wxListBox(fastaPanel, wxID_FastaListBox, wxPoint(-1, -1), wxSize(-1, -1)); 
        flbbox->Add(fastaListBox, 5, wxEXPAND | wxALL, 20);
        ListBoxPanel *fastaBtnPanel = new ListBoxPanel(fastaPanel, fastaListBox, "Fasta", "fasta");
        flbbox->Add(fastaBtnPanel, 1, wxEXPAND | wxRIGHT, 10);
        fastaPanel->SetSizerAndFit(flbbox);
        fastaPanel->Center();
        
        circularCheckBox = new wxCheckBox(this, wxID_CircularCheckBox, "Circular Genome", wxPoint(-1, -1), wxSize(-1, -1));
        circularCheckBox->SetValue(c.circular);
        Connect(wxID_CircularCheckBox, wxEVT_CHECKBOX,
                wxCommandEventHandler(BasicPanel::OnCircularCheck));
        
        fastabox->Add(fastaTitle, 0);
        fastabox->Add(fastaPanel, 1, wxEXPAND);
        fastabox->Add(circularCheckBox, 0, wxEXPAND);
        
        /*
                enzyme box
        */
        wxBoxSizer *enzymebox = new wxBoxSizer(wxVERTICAL);
        wxStaticText *enzymeTitle = new wxStaticText(this, wxID_ANY, wxT("Enzymes"));
        
        wxPanel * ePanel = new wxPanel(this, -1);
        wxBoxSizer *eclbbox = new wxBoxSizer(wxHORIZONTAL);
        enzymeCheckListBox = new wxCheckListBox(ePanel, wxID_EnzymeCheckListBox, wxPoint(-1, -1), wxSize(-1, -1)); 
        eclbbox->Add(enzymeCheckListBox, 5, wxEXPAND | wxALL, 20);
        ePanel->SetSizerAndFit(eclbbox);
        ePanel->Center();
        
        enzymebox->Add(enzymeTitle);
        enzymebox->Add(ePanel, 1, wxEXPAND);
        
        Connect(wxID_EnzymeCheckListBox, wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, 
                wxCommandEventHandler(BasicPanel::OnEnzDblClick));
                
        /*
                length box
        */
        wxBoxSizer *lengthbox = new wxBoxSizer(wxVERTICAL);
        wxStaticText *lengthTitle = new wxStaticText(this, wxID_ANY, wxT("Size distribution"));
        wxBoxSizer *sizedistbox = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText *meanText = new wxStaticText(this, wxID_ANY, wxT("Mean:"));
        sizeMeanCtrl = new wxTextCtrl(this, wxID_SizeMean, c.get("avg_mol_len"), wxPoint(-1, -1), wxSize(-1, -1));
        wxStaticText *stdText = new wxStaticText(this, wxID_ANY, wxT("Standard deviation:"));
        sizeSDCtrl =  new wxTextCtrl(this, wxID_SizeSD, c.get("sd_mol_len"), wxPoint(-1, -1), wxSize(-1, -1));
        sizedistbox->Add(meanText, 0, wxEXPAND | wxALL, 20);
        sizedistbox->Add(sizeMeanCtrl, 0, wxEXPAND | wxTOP | wxBOTTOM, 20);
        sizedistbox->Add(stdText, 0, wxEXPAND | wxALL, 20);
        sizedistbox->Add(sizeSDCtrl, 0, wxEXPAND | wxRIGHT | wxTOP | wxBOTTOM, 20);
        lengthbox->Add(lengthTitle);
        lengthbox->Add(sizedistbox);
        
        Connect(wxID_SizeMean, wxEVT_TEXT,
                wxCommandEventHandler(BasicPanel::OnSizeMean));
        Connect(wxID_SizeSD, wxEVT_TEXT,
                wxCommandEventHandler(BasicPanel::OnSizeSD));
        
        /*
                close box
        */
        wxBoxSizer *closebox = new wxBoxSizer(wxHORIZONTAL);
        wxButton *ok = new wxButton(this, wxID_OK, wxT("Ok"));
        wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
        closebox->Add(ok, 0, wxEXPAND | wxBOTTOM | wxLEFT, 20);
        closebox->Add(cancel, 0, wxEXPAND | wxBOTTOM | wxLEFT, 20);
        
        
        /*
                main box
        */
        mainbox->Add(namebox, 0, wxEXPAND | wxALL, 10);
        mainbox->Add(fastabox, 0, wxEXPAND | wxALL, 10);
        mainbox->Add(enzymebox, 0, wxEXPAND | wxALL, 10);
        mainbox->Add(lengthbox, 0, wxEXPAND | wxALL, 10);
        mainbox->Add(closebox, 0, wxEXPAND | wxALL, 10);
        
        SetSizerAndFit(mainbox);
        
        update();
}

void BasicPanel::addFasta(wxString fasta) {
        c.files.push_back(fasta);
        update();
}

void BasicPanel::removeFasta(wxString fasta) {
        auto it = std::find(c.files.begin(), c.files.end(), fasta);
        if (it != c.files.end()) {
                c.files.erase(it);
        }
        update();
}

void BasicPanel::clearFasta() {
        c.files.clear();
        update();
}

void BasicPanel::update() {
        fastaListBox->Clear();
        for (auto f : c.files) {
                fastaListBox->Append(f);
        }
        
        enzymeCheckListBox->Clear();
        for (auto kv : enzymes) {
                enzymeCheckListBox->Append(kv.first);
        }
        
        for (auto kv : c.enzymes) {
                int idx = enzymeCheckListBox->FindString(kv.first);
                if (idx != wxNOT_FOUND) {
                        enzymeCheckListBox->Check(idx);
                }
        }
}

void BasicPanel::OnEnzDblClick(wxCommandEvent& event)
{
        int sel = enzymeCheckListBox->GetSelection();
        if (sel != -1) {
                wxString id = enzymeCheckListBox->GetString(sel);
                EnzymeDialog dlg(this, wxID_ANY, _("Change label"), enzymes[id]);
                if (dlg.ShowModal() == wxID_CANCEL) {
                        return;
                } else {
                        enzyme e = dlg.GetEnzyme();
                        enzymes[e.id] = e;
                        enzymeCheckListBox->Delete(sel);
                        enzymeCheckListBox->Insert(e.id, sel);
                }
        }
}

void BasicPanel::OnName(wxCommandEvent& Event)
{
        c.set("name", nameCtrl->GetValue());
}

void BasicPanel::OnSizeMean(wxCommandEvent& Event)
{
        c.set("avg_mol_len", sizeMeanCtrl->GetValue());
}

void BasicPanel::OnSizeSD(wxCommandEvent& Event)
{
        c.set("sd_mol_len", sizeSDCtrl->GetValue());
}

void BasicPanel::OnCircularCheck(wxCommandEvent& event) 
{
        c.circular = circularCheckBox->IsChecked();
}