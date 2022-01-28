/*
 * Copyright (C) Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "common.hpp"
#include <unistd.h>
#include "scene_manager.hpp"
#define VERBOSE_LOGGING 1

#if VERBOSE_LOGGING
#define VLOGD LOGE
#else
#define VLOGD
#endif
extern "C" {
void android_main(struct android_app *state);
};
void HandleCommand(int32_t cmd) {
    SceneManager *mgr = SceneManager::GetInstance();

    VLOGD("NativeEngine: handling command %d.", cmd);
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.
            VLOGD("NativeEngine: APP_CMD_SAVE_STATE");

            break;
        case APP_CMD_INIT_WINDOW:
            // We have a window!
            VLOGD("NativeEngine: APP_CMD_INIT_WINDOW");

            VLOGD("HandleComma");
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is going away -- kill the surface
            VLOGD("NativeEngine: APP_CMD_TERM_WINDOW");
            break;
        case APP_CMD_GAINED_FOCUS:
            VLOGD("NativeEngine: APP_CMD_GAINED_FOCUS");
            break;
        case APP_CMD_LOST_FOCUS:
            VLOGD("NativeEngine: APP_CMD_LOST_FOCUS");
            break;
        case APP_CMD_PAUSE:
            VLOGD("NativeEngine: APP_CMD_PAUSE");
            mgr->OnPause();
            break;
        case APP_CMD_RESUME:
            VLOGD("NativeEngine: APP_CMD_RESUME");
            mgr->OnResume();
            break;
        case APP_CMD_STOP:
            VLOGD("NativeEngine: APP_CMD_STOP");
            break;
        case APP_CMD_START:
            VLOGD("NativeEngine: APP_CMD_START");
            break;
        case APP_CMD_WINDOW_RESIZED:
        case APP_CMD_CONFIG_CHANGED:
            VLOGD("NativeEngine: %s", cmd == APP_CMD_WINDOW_RESIZED ?
                                      "APP_CMD_WINDOW_RESIZED" : "APP_CMD_CONFIG_CHANGED");
            // Window was resized or some other configuration changed.
            // Note: we don't handle this event because we check the surface dimensions
            // every frame, so that's how we know it was resized. If you are NOT doing that,
            // then you need to handle this event!
            break;
        case APP_CMD_LOW_MEMORY:
            VLOGD("NativeEngine: APP_CMD_LOW_MEMORY");
            break;
        default:
            VLOGD("NativeEngine: (unknown command).");
            break;
    }

    VLOGD("NativeEngine: STATU");
}

static void _handle_cmd_proxy(struct android_app* app, int32_t cmd) {
  // NativeEngine *engine = (NativeEngine*) app->userData;
   HandleCommand(cmd);
}

void android_main(struct android_app *app) {
    app->onAppCmd=_handle_cmd_proxy;
    while (1) {

        int events;
        struct android_poll_source* source;

        // If not animating, block until we get an event; if animating, don't block.
        while ((ALooper_pollAll( -1, NULL, &events, (void **) &source)) >= 0) {

            // process event
            if (source != NULL) {
                source->process(app, source);
            }

            // are we exiting?
            if (app->destroyRequested) {
                return;
            }
        }
        sleep(1);
//        LOGD("fuck");
//        LOGE("fuck");
    }
}

