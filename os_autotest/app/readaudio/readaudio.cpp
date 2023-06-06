#include "readaudio.h"
#include <stdio.h>
#include <stdlib.h>

int getaudiodevice(){
    int err;                    // error code
    char **hints;               // list of device hints
    char **n;                   // pointer to current hint
    char *name;                 // name of the device
    char *desc;                 // description of the device
    char *ioid;                 // direction (input or output) of the device
    int is_input;               // whether the device is an input device

    // get a list of device hints
    err = snd_device_name_hint(-1, "pcm", (void***)&hints);
    if (err < 0) {
        fprintf(stderr, "Error getting device hints: %s\n", snd_strerror(err));
        return 1;
    }

    // iterate through the list of hints
    n = hints;
    while (*n != NULL) {
        // get the name of the device
        name = snd_device_name_get_hint(*n, "NAME");
        if (name == NULL) {
        continue;
        }

        // get the description of the device
        desc = snd_device_name_get_hint(*n, "DESC");
        // get the direction of the device (input or output)
        ioid = snd_device_name_get_hint(*n, "IOID");
        is_input = (ioid != NULL && strcmp(ioid, "Input") == 0);

        // print the information about the device
        printf("Device: %s\n", name);
        printf("  Description: %s\n", (desc != NULL) ? desc : "None");
        printf("  Direction: %s\n", (is_input) ? "Input" : "Output");

        // free the memory allocated by snd_device_name_get_hint()
        free(name);
        free(desc);
        free(ioid);

        // move to the next hint
        n++;
        }

    // free the memory allocated by snd_device_name_hint()
    snd_device_name_free_hint((void**)hints);

    return 0;
}

#define BUFFER_SIZE 1024  // size of the audio buffer, in frames
#define NUM_CHANNELS 2    // number of channels (e.g., stereo)
#define SAMPLE_RATE 44100 // sample rate in Hz

int main(int argc, char* argv[])
{
    snd_pcm_t *handle; // handle to the audio device
    int rc,err,framenum;           // error code


    // int ret = getaudiodevice();
    // return 0;

    //open the default audio input device         default:CARD=Codec
    rc = snd_pcm_open(&handle,"default", SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) {
        fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
        exit(1);
    }

    err = snd_pcm_set_params(handle,
                           SND_PCM_FORMAT_S16_LE,  // sample format
                           SND_PCM_ACCESS_RW_INTERLEAVED,  // access type
                           NUM_CHANNELS,           // number of channels
                           SAMPLE_RATE,            // sample rate
                           1,                      // allow resampling
                           500000);                // 0.5 seconds latency
    if (err < 0) {
        fprintf(stderr, "Error setting device parameters: %s\n", snd_strerror(err));
        snd_pcm_close(handle);
        return 1;
    }

    // allocate a buffer to hold the audio data
    short *buffer = (short*)malloc(BUFFER_SIZE * NUM_CHANNELS * sizeof(short));
    if (buffer == NULL) {
        fprintf(stderr, "Error allocating buffer\n");
        snd_pcm_close(handle);
        return 1;
    }


    // read audio data from the device
    fprintf(stderr,"start reading\n");
    while (1) {
        // read audio data from the device
        framenum = snd_pcm_readi(handle, buffer, BUFFER_SIZE);
        if (framenum < 0) {
        fprintf(stderr, "Error reading audio data: %s\n", snd_strerror(framenum));
        snd_pcm_close(handle);
        return 1;
        }

        fprintf(stderr,"reading %d frames\n", framenum);
        // process the audio data here...
    }

    // close the audio device
    snd_pcm_close(handle);

    // free the buffer
    free(buffer);

    return 0;
}

// int main() {
//     snd_pcm_t *pcm_handle;
//     int rc;

//     // Open the microphone device
//     rc = snd_pcm_open(&pcm_handle, "pcmC0D0c", SND_PCM_STREAM_CAPTURE, 0);
//     if (rc < 0) {
//         fprintf(stderr, "Error opening PCM device: %s\n", snd_strerror(rc));
//         exit(1);
//     }

//     // Read audio data from the microphone
//     short buffer[128];
//     while (1) {
//         rc = snd_pcm_readi(pcm_handle, buffer, sizeof(buffer) / sizeof(short));
//         if (rc == -EPIPE) {
//             fprintf(stderr, "Overrun occurred\n");
//             snd_pcm_prepare(pcm_handle);
//         } else if (rc < 0) {
//             fprintf(stderr, "Error from read: %s\n", snd_strerror(rc));
//         } else if (rc != (int)sizeof(buffer) / sizeof(short)) {
//             fprintf(stderr, "Short read, read %d bytes\n", rc);
//         }
//     }

//     // Close the microphone device
//     snd_pcm_close(pcm_handle);

//     return 0;
// }

int arecord() {
    int card = -1;
    snd_ctl_t *handle;
    snd_ctl_card_info_t *info;
    int dev = -1;
    snd_pcm_info_t *pcminfo;
    char name[32];
    snd_ctl_card_info_alloca(&info);
    snd_pcm_info_alloca(&pcminfo);
    while (snd_card_next(&card) >= 0 && card >= 0) {
        sprintf(name, "hw:%d", card);
        if (snd_ctl_open(&handle, name, 0) < 0) {
            continue;
        }
        if (snd_ctl_card_info(handle, info) < 0) {
            snd_ctl_close(handle);
            continue;
        }
        printf("card %d: %s [%s], device 0: ", card, snd_ctl_card_info_get_id(info), snd_ctl_card_info_get_name(info));
        dev = -1;
        while (snd_ctl_pcm_next_device(handle, &dev) >= 0 && dev >= 0) {
            snd_pcm_info_set_device(pcminfo, dev);
            snd_pcm_info_set_subdevice(pcminfo, 0);
            snd_pcm_info_set_stream(pcminfo, SND_PCM_STREAM_CAPTURE);
            if (snd_ctl_pcm_info(handle, pcminfo) < 0)
                printf("???");
            else
                printf("%d: %s [%s]\n", dev, snd_pcm_info_get_id(pcminfo), snd_pcm_info_get_name(pcminfo));
        }
        snd_ctl_close(handle);
    }
    return 0;
}

int main0() {
    snd_pcm_t *handle = NULL;
    int rc;

    //arecord();
    //return 0;

    // Open PCM device for recording
    rc = snd_pcm_open(&handle,"default:CARD=Codec", SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) {
        fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
        exit(1);
    }

    //fprintf(stderr, "error handle: %p\n", handle);
    //return 0;
    // Configure the parameters of the PCM device
    // ...

    // Read data from the PCM device
    char *buffer;
    int size;
    rc = snd_pcm_readi(handle, buffer, size);
    if (rc < 0) {
        fprintf(stderr, "error reading from pcm device: %s\n", snd_strerror(rc));
        exit(1);
    }

    // Do something with the audio data in the buffer
    // ...

    // Close the PCM device
    snd_pcm_close(handle);
    return 0;
}

/*
  A Minimal Capture Program
  This program opens an audio interface for capture, configures it for
  stereo, 16 bit, 44.1kHz, interleaved conventional read/write
  access. Then its reads a chunk of random data from it, and exits. It
  isn't meant to be a real program.
  From on Paul David's tutorial : http://equalarea.com/paul/alsa-audio.html
  Fixes rate and buffer problems
  sudo apt-get install libasound2-dev
  gcc -o alsa-record-example -lasound alsa-record-example.c && ./alsa-record-example hw:0
*/

int main1 (int argc, char *argv[])
{
  int i;
  int err;
  char *buffer;
  int buffer_frames = 128;
  unsigned int rate = 44100;
  snd_pcm_t *capture_handle;
  snd_pcm_hw_params_t *hw_params;
	snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;

  if ((err = snd_pcm_open (&capture_handle, argv[1], SND_PCM_STREAM_CAPTURE, 0)) < 0) {
    fprintf (stderr, "cannot open audio device %s (%s)\n",
             argv[1],
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "audio interface opened\n");

  if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
    fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params allocated\n");

  if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
    fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params initialized\n");

  if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    fprintf (stderr, "cannot set access type (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params access setted\n");

  if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, format)) < 0) {
    fprintf (stderr, "cannot set sample format (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params format setted\n");

  if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &rate, 0)) < 0) {
    fprintf (stderr, "cannot set sample rate (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params rate setted\n");

  if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, 2)) < 0) {
    fprintf (stderr, "cannot set channel count (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params channels setted\n");

  if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
    fprintf (stderr, "cannot set parameters (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params setted\n");

  snd_pcm_hw_params_free (hw_params);

  fprintf(stdout, "hw_params freed\n");

  if ((err = snd_pcm_prepare (capture_handle)) < 0) {
    fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "audio interface prepared\n");

  buffer = (char*)malloc(128 * snd_pcm_format_width(format) / 8 * 2);

  fprintf(stdout, "buffer allocated\n");

  for (i = 0; i < 10; ++i) {
    if ((err = snd_pcm_readi (capture_handle, buffer, buffer_frames)) != buffer_frames) {
      fprintf (stderr, "read from audio interface failed (%s)\n",
               err, snd_strerror (err));
      exit (1);
    }
    fprintf(stdout, "read %d done\n", i);
  }

  free(buffer);

  fprintf(stdout, "buffer freed\n");

  snd_pcm_close (capture_handle);
  fprintf(stdout, "audio interface closed\n");

  exit (0);
}
