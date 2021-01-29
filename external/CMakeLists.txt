add_library(stb_image INTERFACE)
add_library(cgltf INTERFACE)

#stb_image
target_include_directories(stb_image INTERFACE stb-master)

#cgltf
target_include_directories(cgltf INTERFACE cgltf-master)
