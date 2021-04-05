#include <BaseApplication.h>
#include <Log/Log.h>

class Application : public Leprechaun::BaseApplication {
public:
  Application(int argc, char **argv) {
    Leprechaun::ApplicationConfig defaultConfig;
    Leprechaun::BaseApplication(argc, argv, defaultConfig);
  }
  ~Application() {

  }

protected:

  void onInitialize() override {

  }

  void onUpdate(const float &delata) { (void) delata; }

  void onDraw() override {}

private:
};


IMPLEMENT_DEMO(Application)
