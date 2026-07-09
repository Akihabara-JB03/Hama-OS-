OUTPUT_FORMAT("binary") /* プレーンなバイナリを出力 */
ENTRY(_start)

SECTIONS
{
    /* PCのBIOSが最初に読み込む 0x7C00 番地からスタート */
    . = 0x7E00;

    .text : {
        *(.text)
    }
    .rodata : { *(.rodata) }
    .data   : { *(.data) }
    .bss    : { *(.bss) }

    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
    }
}
