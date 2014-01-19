/* 
 * Copyright (c) 2013 g.gitau
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions: 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 *
 */
#include "windowbuilder.h"

GObject* wb_build_from_file (GtkBuilder** builder, const gchar* file_path,
        const gchar* window_name)
{
    assert (builder != NULL);
    assert (*builder != NULL);
    assert (file_path != NULL);
    assert (window_name != NULL);

    GObject* window = NULL;
    gtk_builder_add_from_file (*builder, file_path, NULL);
    window = gtk_builder_get_object (*builder, window_name);

    if (window == NULL)
    {
       g_print ("Could not retrieve window witn name %s from path %s\n", 
               window_name,file_path);
    }

    return window;
}
