#include "openmic.h"

// static char card[64] = "default";

// bool SetMicCaptureVolume(int uVolume)
// {
// 	int err;
// 	int orig_volume = 0;
// 	static snd_ctl_t *handle = NULL;
// 	snd_ctl_elem_info_t *info;
// 	snd_ctl_elem_id_t *id;
// 	snd_ctl_elem_value_t *control;
// 	unsigned int count;
// 	snd_ctl_elem_type_t type;
// 	snd_ctl_elem_info_alloca(&info);
// 	snd_ctl_elem_id_alloca(&id);
// 	snd_ctl_elem_value_alloca(&control);

// 	snd_ctl_elem_id_set_interface(id, SND_CTL_ELEM_IFACE_MIXER);/* default */
// 	snd_ctl_elem_id_set_name(id, "Mic Capture Volume");
// //	snd_ctl_elem_id_set_index(id, 1);  // "Mic Capture Volume" does not have index.

// 	if ((err = snd_ctl_open(&handle, card, 0)) < 0) {
// 		//BTLOGERROR(LCBTLib,"Control %s open error: %s/n", card, snd_strerror(err));
// 		return false;
// 	}

// 	snd_ctl_elem_info_set_id(info, id);
// 	if ((err = snd_ctl_elem_info(handle, info)) < 0) {
// 		//BTLOGERROR(LCBTLib,"Cannot find the given element from control %s/n", card);
// 		snd_ctl_close(handle);
// 		handle = NULL;
// 		return false;
// 	}
// 	type = snd_ctl_elem_info_get_type(info);
// 	count = snd_ctl_elem_info_get_count(info);

// 	if(type != SND_CTL_ELEM_TYPE_INTEGER || 1 != count) {
// 		//BTLOGERROR(LCBTLib,"Cannot find the given element from control %s/n", card);
// 		snd_ctl_close(handle);
// 		handle = NULL;
// 		return false;
// 	}

// 	snd_ctl_elem_value_set_id(control, id);

// 	if (!snd_ctl_elem_read(handle, control)) {
// 		orig_volume = snd_ctl_elem_value_get_integer(control, 0);
// 	}

// 	if(uVolume != orig_volume) {
// 		//BTLOGERROR(LCBTLib,"uVolume != orig_volume ##################### new_value(%d) orgin_value(%d)/n",uVolume,orig_volume);
// 		snd_ctl_elem_value_set_integer(control, 0, static_cast<long>(uVolume));
// 		if ((err = snd_ctl_elem_write(handle, control)) < 0) {
// 			//BTLOGERROR(LCBTLib,"Control %s element write error: %s/n", card, snd_strerror(err));
// 			snd_ctl_close(handle);
// 			handle = NULL;
// 			return false;
// 		}
// 	}
// 	snd_ctl_close(handle);
// 	handle = NULL;
// 	return true;
// }

// int main0(int argc, char* argv[])
// {
//     int uVolume = std::stoi(argv[1]);

//     return SetMicCaptureVolume(uVolume);
// }


int main0() {
    int err;
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *hw_params;

    // Open the default capture device
    err = snd_pcm_open(&capture_handle, "default", SND_PCM_STREAM_CAPTURE, 0);
    if (err < 0) {
        printf("Error opening capture device: %s\n", snd_strerror(err));
        return 1;
    }

    // Allocate hardware parameters structure
    snd_pcm_hw_params_alloca(&hw_params);

    // Fill the hardware parameters with default values
    err = snd_pcm_hw_params_any(capture_handle, hw_params);
    if (err < 0) {
        printf("Error filling hardware parameters: %s\n", snd_strerror(err));
        snd_pcm_close(capture_handle);
        return 1;
    }

    unsigned int val_value = 44100;
    unsigned int* val = &val_value;
    int dir_value = 0;
    int* dir = &dir_value;
    // Set the desired hardware parameters
    err = snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    err |= snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S16_LE);
    err |= snd_pcm_hw_params_set_channels(capture_handle, hw_params, 1);
    err |= snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, val, dir);
    if (err < 0) {
        printf("Error setting hardware parameters: %s\n", snd_strerror(err));
        snd_pcm_close(capture_handle);
        return 1;
    }

    // Apply the hardware parameters
    err = snd_pcm_hw_params(capture_handle, hw_params);
    if (err < 0) {
        printf("Error applying hardware parameters: %s\n", snd_strerror(err));
        snd_pcm_close(capture_handle);
        return 1;
    }

    // Start capturing audio
    err = snd_pcm_prepare(capture_handle);
    if (err < 0) {
        printf("Error preparing capture device: %s\n", snd_strerror(err));
        snd_pcm_close(capture_handle);
        return 1;
    }

    // Capture audio data
    short buffer[1024];
    while (1) {
        err = snd_pcm_readi(capture_handle, buffer, 1024);
        if (err < 0) {
            printf("Error reading audio data: %s\n", snd_strerror(err));
            snd_pcm_close(capture_handle);
            return 1;
        }
        printf("readi\n");
        // Process the captured audio data here
    }

    // Close the capture device
    snd_pcm_close(capture_handle);

    return 0;
}

int main2() {
    const char *device = "default"; // or specify your own device here
    const char *control_name = "Mic1";
    int current_value, err;

    snd_ctl_t *ctl;
    snd_ctl_elem_id_t *id;
    snd_ctl_elem_value_t *value;

    // Open the control interface for the default sound card device
    err = snd_ctl_open(&ctl, device, 0);
    if (err < 0) {
        printf("Error opening control interface: %s\n", snd_strerror(err));
        return 1;
    }

    // Allocate memory for the control element ID and initialize it
    snd_ctl_elem_id_alloca(&id);
    snd_ctl_elem_id_set_interface(id, SND_CTL_ELEM_IFACE_MIXER);
    snd_ctl_elem_id_set_name(id, control_name);

    // Get the control element by ID
    // snd_hctl_t *hctl;
    // snd_hctl_elem_t *elems;
    // if ((err = snd_hctl_open(&hctl, device, 0)) < 0) {
    //     printf("Control %s open error: %s\n", device, snd_strerror(err));
    //     return err;
    // }
    // if ((err = snd_hctl_load(hctl)) < 0) {
    //     printf("Control %s load error: %s\n", device, snd_strerror(err));
    //     return err;
    // }
    // elems = snd_hctl_find_elem(hctl, id);
    // if (!elems) {
    //     printf("Control element not found: %s\n", control_name);
    //     snd_hctl_close(hctl);
    //     return 1;
    // }

    // Allocate memory for the control element value and initialize it
    snd_ctl_elem_value_alloca(&value);
    snd_ctl_elem_value_set_id(value, id);

    // Read the current value of the control element
    err = snd_ctl_elem_read(ctl, value);
    if (err < 0) {
        printf("Error reading control element value: %s\n", snd_strerror(err));
        snd_ctl_close(ctl);
        return 1;
    }

    // Get the current value of the control element
    current_value = snd_ctl_elem_value_get_integer(value, 0);

    // Set the new value of the control element
    snd_ctl_elem_value_set_integer(value, 0, current_value + 1);

    // Write the new value to the control element
    err = snd_ctl_elem_write(ctl, value);
    if (err < 0) {
        printf("Error writing control element value: %s\n", snd_strerror(err));
        snd_ctl_close(ctl);
        return 1;
    }

    // Close the control interface
    snd_ctl_close(ctl);

    return 0;
}

int cset(int argc, char *argv[], int roflag, int keep_handle)
{
    int ignore_error = 0;
    int quiet = 0;
	int err;
    char card[64] = "default";
	static snd_ctl_t *handle = NULL;
	snd_ctl_elem_info_t *info;
	snd_ctl_elem_id_t *id;
	snd_ctl_elem_value_t *control;
	snd_ctl_elem_info_alloca(&info);
	snd_ctl_elem_id_alloca(&id);
	snd_ctl_elem_value_alloca(&control);

	if (argc < 1) {
		fprintf(stderr, "Specify a full control identifier: [[iface=<iface>,][name='name',][index=<index>,][device=<device>,][subdevice=<subdevice>]]|[numid=<numid>]\n");
		return -EINVAL;
	}
	if (snd_ctl_ascii_elem_id_parse(id, argv[0])) {
		fprintf(stderr, "Wrong control identifier: %s\n", argv[0]);
		return -EINVAL;
	}
	// if (debugflag) {
	// 	printf("VERIFY ID: ");
	// 	show_control_id(id);
	// 	printf("\n");
	// }
	if (handle == NULL &&
	    (err = snd_ctl_open(&handle, card, 0)) < 0) {
		printf("Control %s open error: %s\n", card, snd_strerror(err));
		return err;
	}
	snd_ctl_elem_info_set_id(info, id);
	if ((err = snd_ctl_elem_info(handle, info)) < 0) {
		if (ignore_error)
			return 0;
		printf("Cannot find the given element from control %s\n", card);
		if (! keep_handle) {
			snd_ctl_close(handle);
			handle = NULL;
		}
		return err;
	}
	snd_ctl_elem_info_get_id(info, id);     /* FIXME: Remove it when hctl find works ok !!! */
	if (!roflag) {
		snd_ctl_elem_value_set_id(control, id);
		if ((err = snd_ctl_elem_read(handle, control)) < 0) {
			if (ignore_error)
				return 0;
			printf("Cannot read the given element from control %s\n", card);
			if (! keep_handle) {
				snd_ctl_close(handle);
				handle = NULL;
			}
			return err;
		}
		err = snd_ctl_ascii_value_parse(handle, control, info, argv[1]);
		if (err < 0) {
 			if (!ignore_error)
				printf("Control %s parse error: %s\n", card, snd_strerror(err));
			if (!keep_handle) {
				snd_ctl_close(handle);
				handle = NULL;
			}
			return ignore_error ? 0 : err;
		}
		if ((err = snd_ctl_elem_write(handle, control)) < 0) {
			if (!ignore_error)
				printf("Control %s element write error: %s\n", card, snd_strerror(err));
			if (!keep_handle) {
				snd_ctl_close(handle);
				handle = NULL;
			}
			return ignore_error ? 0 : err;
		}
	}
	if (! keep_handle) {
		snd_ctl_close(handle);
		handle = NULL;
	}
	// if (!quiet) {
	// 	snd_hctl_t *hctl;
	// 	snd_hctl_elem_t *elem;
	// 	if ((err = snd_hctl_open(&hctl, card, 0)) < 0) {
	// 		printf("Control %s open error: %s\n", card, snd_strerror(err));
	// 		return err;
	// 	}
	// 	if ((err = snd_hctl_load(hctl)) < 0) {
	// 		printf("Control %s load error: %s\n", card, snd_strerror(err));
	// 		return err;
	// 	}
	// 	elem = snd_hctl_find_elem(hctl, id);
	// 	if (elem)
	// 		show_control("  ", elem, LEVEL_BASIC | LEVEL_ID);
	// 	else
	// 		printf("Could not find the specified element\n");
	// 	snd_hctl_close(hctl);
	// }
	return 0;
}

int main1(){

    int roflag=0, keep_handle=0;

    cset(argc - optind - 1, argc - optind > 1 ? argv + optind + 1 : NULL, 1, 0);
}
