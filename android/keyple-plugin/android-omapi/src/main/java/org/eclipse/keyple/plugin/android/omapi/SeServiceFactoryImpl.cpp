#include "SeServiceFactoryImpl.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace android {
                    namespace omapi {
                        using org::simalliance::openmobileapi::SEService;
                        using android::app::Application;
                        using android::content::Context;
                        using android::content::pm::PackageInfo;
                        using android::content::pm::PackageManager;
                        using android::util::Log;
std::string SeServiceFactoryImpl::TAG = SeServiceFactoryImpl::typeid->getSimpleName();

                        std::shared_ptr<SEService> SeServiceFactoryImpl::connectToSe(std::shared_ptr<SEService::CallBack> callBack) {
                            if (isEnvironmentReady()) {
                                Log::w(TAG, "Environment is ready for OMAPI, connecting to SeService");
                                return std::make_shared<SEService>(getApplicationContext(), callBack);
                            }
                            else {
                                Log::w(TAG, "Environment is not ready for OMAPI");
                                return nullptr;
                            }
                        }

                        Boolean SeServiceFactoryImpl::isEnvironmentReady() {
                            return getOMAPIVersion(getApplicationContext()) != "";
                        }

                        std::shared_ptr<Application> SeServiceFactoryImpl::getApplicationContext() {
                            std::shared_ptr<Application> app = nullptr;

                            Log::i(TAG, "Retrieving Application Context with reflection android.app.AppGlobals");

                            try {

                                app = std::static_pointer_cast<Application>(std::type_info::forName("android.app.ActivityThread").getMethod("currentApplication").invoke(nullptr, static_cast<std::vector<std::shared_ptr<void>>>(nullptr)));
                            }
                            catch (const IllegalAccessException &e) {
                                e->printStackTrace();
                            }
                            catch (const InvocationTargetException &e) {
                                e->printStackTrace();
                            }
                            catch (const NoSuchMethodException &e) {
                                e->printStackTrace();
                            }
                            catch (const ClassNotFoundException &e) {
                                e->printStackTrace();
                            }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to the exception 'finally' clause:
                            finally {
                                return app;
                            }

                        }

                        std::string SeServiceFactoryImpl::getOMAPIVersion(std::shared_ptr<Context> context) {
                            try {
                                std::shared_ptr<PackageInfo> packageInfo = context->getPackageManager().getPackageInfo("android.smartcard", 0);
                                return packageInfo->versionName;
                            }
                            catch (const NameNotFoundException &e1) {
                                try {
                                    std::shared_ptr<PackageInfo> packageInfo = context->getPackageManager().getPackageInfo("org.simalliance.openmobileapi.service", 0);
                                    return packageInfo->versionName;
                                }
                                catch (const NameNotFoundException &e2) {
                                    try {
                                        std::shared_ptr<PackageInfo> packageInfo = context->getPackageManager().getPackageInfo("com.sonyericsson.smartcard", 0);
                                        return packageInfo->versionName;
                                    }
                                    catch (const NameNotFoundException &e3) {
                                        return "";
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
