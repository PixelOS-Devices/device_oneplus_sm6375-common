#
# Copyright (C) 2023 StatiXOS
# Copyright (C) 2024 Paranoid Android
# SPDX-License-Identifier: Apache-2.0
#

# Custom tuning for Blair SoC

# Runtime fs tuning
echo 128 > /sys/block/sda/queue/read_ahead_kb
echo 128 > /sys/block/sda/queue/nr_requests
echo 1 > /sys/block/sda/queue/iostats
echo 128 > /sys/block/dm-0/queue/read_ahead_kb
echo 128 > /sys/block/dm-1/queue/read_ahead_kb
echo 128 > /sys/block/dm-2/queue/read_ahead_kb
echo 128 > /sys/block/dm-3/queue/read_ahead_kb
echo 128 > /sys/block/dm-4/queue/read_ahead_kb
echo 128 > /sys/block/dm-5/queue/read_ahead_kb
echo 128 > /sys/block/dm-6/queue/read_ahead_kb
echo 128 > /sys/block/dm-7/queue/read_ahead_kb
echo 128 > /sys/block/dm-8/queue/read_ahead_kb
echo 128 > /sys/block/dm-9/queue/read_ahead_kb

# cpuset
echo 0-1 > /dev/cpuset/background/cpus
echo 0-3 > /dev/cpuset/system-background/cpus
echo 0-3 > /dev/cpuset/restricted/cpus
echo 1-2 > /dev/cpuset/audio-app/cpus

# Sched
echo 95 > /proc/sys/kernel/sched_upmigrate
echo 85 > /proc/sys/kernel/sched_downmigrate

function write_irq_affinity() {
    # Arguments:
    # $1 = irq name
    # $2 = cpu id
    irq_dir="$(dirname /proc/irq/*/$1)"
    [ -d "$irq_dir" ] && echo $2 > "${irq_dir}/smp_affinity_list"
}

# IRQ Tuning
# kgsl_3d0_irq -> CPU 1
# msm_drm -> CPU 2
write_irq_affinity kgsl_3d0_irq 1
write_irq_affinity msm_drm 2
