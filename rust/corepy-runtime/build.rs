// Build script to link C++ kernels
fn main() {
    // Tell cargo to link against the C++ kernel library
    // The library is built by CMake in csrc/
    
    let manifest_dir = std::env::var("CARGO_MANIFEST_DIR").unwrap();
    let repo_root = std::path::Path::new(&manifest_dir)
        .parent()
        .unwrap()
        .parent()
        .unwrap();
    
    let csrc_path = repo_root.join("csrc");
    let build_path = csrc_path.join("build");
    
    // Check if C++ library is built
    if !build_path.exists() {
        eprintln!("WARNING: C++ kernels not built!");
        eprintln!("Run: cd csrc && cmake -B build && cmake --build build");
        return;
    }
    
    // Link the C++ library
    println!("cargo:rustc-link-search=native={}", build_path.display());
    println!("cargo:rustc-link-lib=static=corepy_kernels");
    
    // Link OpenBLAS (required by corepy_kernels)
    println!("cargo:rustc-link-search=native=/usr/lib/x86_64-linux-gnu");
    println!("cargo:rustc-link-lib=dylib=openblas");
    
    // Link C++ standard library
    println!("cargo:rustc-link-lib=dylib=stdc++");
    
    // Tell cargo to rerun if C++ code changes
    println!("cargo:rerun-if-changed=../../csrc/src");
    println!("cargo:rerun-if-changed=../../csrc/include");
}

