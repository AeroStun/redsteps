#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/scale.h>

#include "libredsteps.hxx"

class RedSteps : public Gtk::ApplicationWindow {
    
    Gtk::Box box;
    Gtk::Button btn;
    Gtk::Scale slider;
    
public:
    RedSteps()
    : box(Gtk::ORIENTATION_HORIZONTAL, 10), btn("Reset"), slider{} {
        set_title("RedSteps-GTK [XF86VM]");
        set_default_size(520, 50);
        // FIXME consider case where display fails to connect
        
        btn.signal_clicked().connect(
        [this]() {
          if (slider.get_value() == 6500.0)
            update_display();
          else
            slider.set_value(6500.0);
        });
        
        slider.set_draw_value();
        slider.set_show_fill_level(false);
        slider.set_range(1000.0, 25000.0);
        //slider.add_mark(6500.0, Gtk::PositionType::POS_TOP, {});
        slider.set_value(6500.0);
        slider.set_increments(1.0, 10.0);
        slider.set_round_digits(0);
        slider.signal_value_changed().connect([this]{ update_display(); });

        box.pack_start(btn, Gtk::PACK_SHRINK);
        box.pack_start(slider, Gtk::PACK_EXPAND_WIDGET);
        add(box);
        show_all();
    }
    
    void update_display() {
      redsteps::Display display;
      display.set_crude_temperature(static_cast<redsteps::Kelvin>(slider.get_value()));
    }
};

int main(int argc, char** argv) {
     auto app = Gtk::Application::create(argc, argv, "dev.aerostun.redsteps-gtk.RedSteps");
     RedSteps hw;
     return app->run(hw);
}
