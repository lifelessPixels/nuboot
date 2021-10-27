#include <efi/efi.h>
#include <efi/protocols/simpletextoutput.h>
#include <efi/protocols/loadedimage.h>

extern "C"
void nuboot_main(efi::handle image_handle, efi::system_table *system_table) {

    (void)(image_handle);
    efi::boot_services *boot_services = system_table->boot_services;
    efi::protocols::simple_text_output *text_output = system_table->console_out;
    system_table->console_out->clear_screen(system_table->console_out);

    efi::protocols::device_path_to_text *device_path_to_text;
    usz number_of_handles = 0;
    efi::handle *handle_buffer = nullptr;
    efi::guid device_path_to_text_guid = efi::protocols::device_path_to_text::protocol_guid;
    efi::status status = boot_services->locate_handle_buffer(efi::locate_search_type::by_protocol, &device_path_to_text_guid, nullptr, &number_of_handles, &handle_buffer);
    if(!status && number_of_handles >= 1) {

        // try to open protocol
        status = boot_services->open_protocol(handle_buffer[0], &device_path_to_text_guid, (void**)(&device_path_to_text), image_handle, nullptr, 1); // TODO: make definitions for attributes 
        if(!status) {

            // protocol handled and opened
            text_output->output_string(system_table->console_out, L"found and opened efi_device_path_to_text_protocol\r\n");

            // // try to open loaded_image_path
            // efi::protocols::loaded_image_path *loaded_image_path;
            // efi::guid loaded_image_path_guid = efi::protocols::loaded_image_path::protocol_guid;
            // status = boot_services->handle_protocol(image_handle, &loaded_image_path_guid, (void**)(&loaded_image_path));

            // if(!status) {

            //     // protocol handled and opened
            //     text_output->output_string(text_output, device_path_to_text->convert_device_path_to_text(loaded_image_path, false, false));

            // }

            // try to list all device paths in system
            efi::guid device_path_guid = efi::protocols::device_path::protocol_guid;
            status = boot_services->locate_handle_buffer(efi::locate_search_type::by_protocol, &device_path_guid, nullptr, &number_of_handles, &handle_buffer);
            if(!status) {

                // for every handle, try to open protocol and print device path
                for(usz i = 0; i < number_of_handles; i++) {

                    efi::protocols::device_path *device_path = nullptr;
                    status = boot_services->open_protocol(handle_buffer[i], &device_path_guid, (void**)(&device_path), image_handle, nullptr, 1);
                    if(!status) {

                        // print device path
                        text_output->output_string(text_output, device_path_to_text->convert_device_path_to_text(device_path, false, true));
                        text_output->output_string(text_output, L"\r\n");

                        // close protocol
                        status = boot_services->close_protocol(handle_buffer[i], &device_path_guid, image_handle, nullptr);

                    }

                }
                

            }

        }

    }

    for(;;);

}