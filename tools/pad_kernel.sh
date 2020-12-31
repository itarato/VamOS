KERNEL_BIN_SIZE=`stat -c%s kernel.bin`
KERNEL_SPACE=24576
PADDING=$(($KERNEL_SPACE-$KERNEL_BIN_SIZE))

head -c $PADDING < /dev/zero > kernel_padding
