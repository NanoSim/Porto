#include <QtCore>
#include <softc/softc.h>
#include "sim.h"
#include "sim-private.h"
#include "sim-handle.h"
#include "config.h"

namespace {
  using SIM_fn_prototype          = void(*)(sim_handle_t*,void*);
  using SIM_timestepper_prototype = void(*)(sim_handle_t*,
					    SIM_fn_prototype,
					    SIM_fn_prototype,
					    SIM_fn_prototype,
					    void*);

  class SIMCommon
  {
  public:
    SIMCommon(){}
    ~SIMCommon(){}

    void loadPlugins() {      
      QDir pluginDir(PLUGIN_DIR);
      foreach (auto const &file, pluginDir.entryList (QDir::Files)) {
	if (QLibrary::isLibrary (file)) {
	  auto lib = QSharedPointer <QLibrary>(new QLibrary(pluginDir.absoluteFilePath(file)));
	  if (lib->resolve("SIM_init") != nullptr) {
	    libList.push_back(lib);	    
	  } else {
	    qDebug() << lib->errorString();
	  }
	}
      }     
    }

    int count() const {
      return libList.count();
    }

    void reg(_sim_application_t *app) const
    {
      qDebug() << "#Registering simulation: " << libList.count();
      foreach (auto lib, libList) {
	SIM_fn_prototype initFunction                 = (SIM_fn_prototype) lib->resolve("SIM_init");
	SIM_fn_prototype computeFunction              = (SIM_fn_prototype) lib->resolve("SIM_compute");
	SIM_timestepper_prototype timeStepperFunction = (SIM_timestepper_prototype) lib->resolve("SIM_time_stepper");
	SIM_fn_prototype cleanupFunction              = (SIM_fn_prototype) lib->resolve("SIM_finalize");

	if (initFunction && computeFunction && timeStepperFunction && cleanupFunction) {
	  sim_register_init(app, initFunction);
	  sim_register_compute(app, computeFunction);
	  sim_register_time_stepper(app, timeStepperFunction);
	  sim_register_finalize(app, cleanupFunction);
	} else {
	  qDebug() << "Unable to run example";
	}
      }
    }
    
  private:
    QList<QSharedPointer <QLibrary> > libList;
  };
  
  static softc_t *softapp = nullptr;  
  static SIMCommon *simCommon = nullptr;
}

sim_application_t* sim_init(int *argc, char **argv)
{ 
  if (softapp == nullptr) {
    softapp = softc_init(*argc, argv);
    sim_application_t * ret;
    ret = new sim_application_t;
    ret->ctx = sim_handle_create();
    ret->user_init = NULL;
    ret->user_pre_init = NULL;
    ret->user_post_init = NULL;
    ret->user_compute = NULL;
    ret->user_time_stepper = NULL;
    ret->user_begin_iter = NULL;
    ret->user_end_iter = NULL;
    ret->user_finalize = NULL;
    ret->user_pre_finalize = NULL;
    ret->user_post_finalize = NULL;

    return ret;
  }

  return NULL;
}

void sim_load (sim_application_t* application)
{
  simCommon = new SIMCommon();
  simCommon->loadPlugins();
  simCommon->reg(application);
}


void sim_run (sim_application_t* application, void *user_data)
{
  if (application == NULL) {
    fprintf(stderr, "sim_init (int*, char**) needs to be called first\n");
    return;
  }

  if (application->user_pre_init != NULL) {
    (*application->user_pre_init)(application->ctx, user_data);
  }

  if (application->user_init != NULL) {
    (*application->user_init)(application->ctx, user_data);
  } else {
    fprintf(stderr, "warning - user_init function is not defined\n");
  }

  if (application->user_post_init != NULL) {
    (*application->user_post_init)(application->ctx, user_data);
  }
  
  if (application->user_time_stepper != NULL && application->user_compute != NULL) {
    (*application->user_time_stepper)(application->ctx, application->user_compute, application->user_begin_iter, application->user_end_iter, user_data);
  } else if (application->user_compute != NULL) {
    (*application->user_compute)(application->ctx, user_data);
  } else {
    fprintf(stderr, "warning - user_compute function is not defined\n");
  }

  if (application->user_pre_finalize != NULL) {
    (*application->user_pre_finalize)(application->ctx, user_data);
  }

  if (application->user_finalize != NULL) {
    (*application->user_finalize)(application->ctx, user_data);
  } else {
    fprintf(stderr, "warning - user_finalize function is not defined\n");
  }
  
  if (application->user_post_finalize != NULL) {
    (*application->user_post_finalize)(application->ctx, user_data);
  }    
}

void sim_register_pre_init    (sim_application_t* application, SIM_fn_prototype fn)
{
  application->user_pre_init = fn;
}

void sim_register_post_init    (sim_application_t* application, SIM_fn_prototype fn)
{
  application->user_post_init = fn;
}

void sim_register_init    (sim_application_t* application, SIM_fn_prototype fn)
{
  application->user_init = fn;
}

void sim_register_compute (sim_application_t* application, SIM_fn_prototype fn)
{
  application->user_compute = fn;
}

void sim_register_time_stepper (sim_application_t* application, SIM_timestepper_prototype fn)
{
  application->user_time_stepper = fn;
}

void sim_register_begin_iter(sim_application_t* application, SIM_fn_prototype fn)
{
  application->user_begin_iter = fn;
}

void sim_register_end_iter(sim_application_t* application, SIM_fn_prototype fn)
{
  application->user_end_iter = fn;
}

void sim_register_finalize(sim_application_t* application, SIM_fn_prototype fn)
{
  application->user_finalize = fn;
}

void sim_register_pre_finalize(sim_application_t* application, SIM_fn_prototype fn)
{
  application->user_pre_finalize = fn;
}

void sim_register_post_finalize(sim_application_t* application, SIM_fn_prototype fn)
{
  application->user_post_finalize = fn;
}

void sim_register_toolkit (sim_application_t *application, const char* filename)
{
  
}
