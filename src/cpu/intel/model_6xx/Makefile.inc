ramstage-y += model_6xx_init.c

cpu_microcode-$(CONFIG_CPU_MICROCODE_CBFS_GENERATE) += microcode_blob.c
