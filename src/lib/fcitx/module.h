/***************************************************************************
 *   Copyright (C) 2010~2010 by CSSlayer                                   *
 *   wengxt@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.              *
 ***************************************************************************/

/**
 * @addtogroup Fcitx
 * @{
 */

#ifndef _FCITX_MODULE_H
#define _FCITX_MODULE_H
#include <fcitx-config/fcitx-config.h>
#include <fcitx-utils/utarray.h>
#include <fcitx/addon.h>

#ifdef __cplusplus
extern "C" {
#endif

    struct _FcitxInstance;

    /**
     * A misc module in Fcitx, it can register hook, or add it's own event
     *        to Fcitx main loop.
     **/
    typedef struct _FcitxModule {
        /**
         * construction function
         */
        void* (*Create)(struct _FcitxInstance* instance);
        /**
         * set main loop watch fd, no need to implement
         */
        void (*SetFD)(void*);
        /**
         * main loop event handle, no need to implement
         */
        void (*ProcessEvent)(void*);
        /**
         * destruct function
         */
        void (*Destroy)(void*);
        /**
         * reload config, no need to implement
         */
        void (*ReloadConfig)(void*);
    } FcitxModule;

    /**
     * the argument to invoke module function
     **/
    typedef struct _FcitxModuleFunctionArg {
        /**
         * arguments
         **/
        void* args[10];
    } FcitxModuleFunctionArg;
    typedef void *(*FcitxModuleFunction)(void *arg, FcitxModuleFunctionArg);

    /**
     * load all modules
     *
     * @param instance fcitx instance
     * @return void
     **/
    void FcitxModuleLoad(struct _FcitxInstance* instance);

    /**
     * invode inter module function wiht addon pointer, returns NULL when fails (the function itself can also return NULL)
     *
     * @param addon addon
     * @param functionId function index
     * @param args arguments
     * @return void*
     **/
    void* FcitxModuleInvokeFunction(struct _FcitxAddon* addon, int functionId, FcitxModuleFunctionArg args);
#define FcitxModuleInvokeVaArgs(addon, functionId, ARGV...)             \
    (FcitxModuleInvokeFunction(addon, functionId,                       \
                               (FcitxModuleFunctionArg){ .args = {ARGV} }))

    /**
     * invoke inter module function with addon name, returns NULL when fails (the function itself can also return NULL)
     *
     * @param instance fcitx instance
     * @param name addon name
     * @param functionId function index
     * @param args arguments
     * @return void*
     **/
    void* FcitxModuleInvokeFunctionByName(struct _FcitxInstance* instance, const char* name, int functionId, FcitxModuleFunctionArg args);
#define FcitxModuleInvokeVaArgsByName(instance, name, functionId, ARGV...) \
    (FcitxModuleInvokeFunctionByName(instance, name, functionId,        \
                               (FcitxModuleFunctionArg){ .args = {ARGV} }))

/** call a function provides by other addon */
#define InvokeFunction(INST, MODULE, FUNC, ARG)  \
    ((MODULE##_##FUNC##_RETURNTYPE) FcitxModuleInvokeFunctionByName(INST, MODULE##_NAME, MODULE##_##FUNC, ARG))

#define InvokeVaArgs(INST, MODULE, FUNC, ARGV...)                       \
    ((MODULE##_##FUNC##_RETURNTYPE)FcitxModuleInvokeFunctionByName(     \
        INST, MODULE##_NAME, MODULE##_##FUNC,                           \
        (FcitxModuleFunctionArg){ .args = {ARGV} }))
/**
 * NOTE: (int)(sizeof((void*[]){ARGV}) / sizeof(void*)) can be used to get
 * the number of arguements. (can be useful for removing the argument number
 * limit) as well as directly call the function without a FunctionArg wrapper.
 **/

/** add a function to current addon */
    static inline void
    AddFunction(struct _FcitxAddon *addon, FcitxModuleFunction func)
    {
        void *temp = (void*)func;
        utarray_push_back(&addon->functionList, &temp);
    }

#ifdef __cplusplus
}
#endif

#endif
/**
 * @}
 */
// kate: indent-mode cstyle; space-indent on; indent-width 0;
