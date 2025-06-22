use std::mem::{size_of, align_of};
use memoffset::offset_of;
#[repr(C)]
struct Example {
    a: u8,
    b: u32,
    c: u16,
}

fn main() {
    println!("Size of u8:    {}", size_of::<u8>());
    println!("Alignment u8:  {}", align_of::<u8>());

    println!("Size of u16:   {}", size_of::<u16>());
    println!("Alignment u16: {}", align_of::<u16>());

    println!("Size of u32:   {}", size_of::<u32>());
    println!("Alignment u32: {}", align_of::<u32>());

    println!("Size of Example struct:    {}", size_of::<Example>());
    println!("Alignment of Example struct: {}", align_of::<Example>());

    println!("Offset of a: {}", offset_of!(Example, a));
    println!("Offset of b: {}", offset_of!(Example, b));
    println!("Offset of c: {}", offset_of!(Example, c));
}
