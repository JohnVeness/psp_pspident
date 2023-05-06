#include <pspsdk.h>
#include <pspkernel.h>
#include <pspctrl.h> // sceCtrl*()
#include <stdio.h> // sprintf()
#include <string.h> // mem*()

#define VER_MAJOR 1
#define VER_MINOR 0
#define VER_BUILD ""

PSP_MODULE_INFO("ident", 0, VER_MAJOR, VER_MINOR);
PSP_MAIN_THREAD_ATTR(0);
PSP_HEAP_SIZE_KB(1024);

#include "../kernel_prx/kernel_prx.h"
#define printf pspDebugScreenPrintf

//SceCtrlData pad;

int main(int argc, char*argv[]) {
	pspDebugScreenInit();
	pspDebugScreenClear();
	printf("\n pspIdent v%i.%i%s by Yoti\n\n", VER_MAJOR, VER_MINOR, VER_BUILD);

	SceUID mod = pspSdkLoadStartModule("kernel.prx", PSP_MEMORY_PARTITION_KERNEL);
	if (mod < 0) {
		printf("Error: LoadStart() returned 0x%08x\n", mod);
		sceKernelDelayThread(3*1000*1000);
		sceKernelExitGame();
	}

	unsigned int firmware = sceKernelDevkitVersion();
	unsigned int tachyon = prxSysregGetTachyonVersion();
	unsigned int baryon; prxSysconGetBaryonVersion(&baryon);
	unsigned int pommel; prxSysconGetPommelVersion(&pommel);
	unsigned int generation = prxKernelGetModel() + 1; // char

	char model[32]; memset(model, 0, sizeof(model));
	char tlotr[16]; memset(tlotr, 0, sizeof(tlotr));

	switch(tachyon) {
		case 0x00140000: {
			sprintf(model, "PSP-1000 TA-079v%x", baryon >> 16);
			sprintf(tlotr, "First");
			break;
		}
		case 0x00200000: {
			sprintf(model, "PSP-1000 TA-079v%x", (baryon >> 16) + 1);
			sprintf(tlotr, "First");
			break;
		}
		case 0x00300000: {
			sprintf(model, "PSP-1000 TA-081v%x", (pommel & 0xF) - 2);
			sprintf(tlotr, "First");
			break;
		}
		case 0x00400000: {
			sprintf(model, "PSP-1000 TA-08%s", baryon == 0x00114000 ? "2" : baryon == 0x00121000 ? "6" : "?");
			sprintf(tlotr, "Legolas%s", baryon == 0x00114000 ? "1" : baryon == 0x00121000 ? "2" : "?");
			break;
		}

		case 0x00500000: {
			switch(baryon) {
				case 0x0022B200: {
					sprintf(model, "PSP-2000 TA-085v1");
					sprintf(tlotr, "Frodo");
					break;
				}
				case 0x00234000: {
					sprintf(model, "PSP-2000 TA-085v2");
					sprintf(tlotr, "Frodo");
					break;
				}
				case 0x00243000: {
					switch(pommel) {
						case 0x00000123: {
							sprintf(model, "PSP-2000 TA-088v1/v2");
							sprintf(tlotr, "Frodo");
							break;
						}
						case 0x00000132: {
							sprintf(model, "PSP-2000 TA-090v1");
							sprintf(tlotr, "Frodo");
							break;
						}
					}
					break;
				}
			}
			break;
		}

		case 0x00600000: {
			switch(baryon) {
				case 0x00243000: {
					sprintf(model, "PSP-2000 TA-088v3");
					sprintf(tlotr, "Frodo");
					break;
				}
				case 0x00263100: {
					sprintf(model, "PSP-3000 TA-090v%x", pommel & 0xF);
					sprintf(tlotr, "Samwise");
					break;
				}
				case 0x00285000: {
					sprintf(model, "PSP-3000 TA-092");
					sprintf(tlotr, "Samwise");
					break;
				}
			}
			break;
		}
		case 0x00810000: {
			switch(baryon) {
				case 0x002C4000: {
					sprintf(model, "PSP-3000 TA-093v%s", pommel == 0x00000141 ? "1" : baryon == 0x00000143 ? "2" : "?");
					sprintf(tlotr, "Samwise VA2");
					break;
				}
				case 0x002E4000: {
					sprintf(model, "PSP-3000 TA-095v1");
					sprintf(tlotr, "Samwise VA2");
					break;
				}
			}
			break;
		}
		case 0x00820000: {
			sprintf(model, "PSP-3000 TA-095v2");
			sprintf(tlotr, "Samwise VA2");
			break;
		}

		case 0x00720000: {
			sprintf(model, "PSP-N1000 TA-091");
			sprintf(tlotr, "Strider");
			break;
		}

		case 0x00900000: {
			sprintf(model, "PSP-E1000 TA-096/TA-097");
			sprintf(tlotr, "Bilbo");
			break;
		}

		default: {
			sprintf(model, "PSP-???? TA-???");
			sprintf(tlotr, "???");
			break;
		}
	}

	if (generation == 4) {
		if (tachyon >> 20 == 8)
			sprintf(model, "%s (fake %02ig)", model, generation);
		else
			sprintf(model, "%s (real %02ig)", model, generation);
	} else {
		sprintf(model, "%s (%02ig)", model, generation);
	}

	printf(" * %-10s 0x%08x (", "Firmware", firmware);
	printf("%x.%x%x)\n", firmware >> 24, (firmware >> 16) & 0xff, (firmware >> 8) & 0xff);
	printf(" * %-10s 0x%08x\n", "Tachyon", tachyon);
	printf(" * %-10s 0x%08x\n", "Baryon", baryon);
	printf(" * %-10s 0x%08x\n", "Pommel", pommel);
	printf(" * %-10s %02ig\n\n", "Generation", generation);

	printf(" * %s\n", model);
	printf(" * Call me [%s], Gandalf!\n", tlotr);

	sceKernelDelayThread(5*1000*1000);
	sceKernelExitGame();

	return 0;
}
