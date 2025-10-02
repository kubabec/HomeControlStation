#!/usr/bin/env python3
import os
import time
import subprocess
import sys
Import("env")

def get_build_flag_value(flag_name):
    build_flags = env.ParseFlags(env['BUILD_FLAGS'])
    flags_with_value_list = [build_flag for build_flag in build_flags.get('CPPDEFINES') if type(build_flag) == list]
    defines = {k: v for (k, v) in flags_with_value_list}
    return defines.get(flag_name)

def write_build_info(project_dir):
    out_dir = os.path.join(project_dir, "src")
    try:
        if not os.path.isdir(out_dir):
            os.makedirs(out_dir, exist_ok=True)
    except Exception as e:
        print("ERROR: nie mogę stworzyć katalogu:", out_dir, e)
        return False

    # timestamp (lokalny czas)
    timestamp = time.strftime("%Y-%m-%d %H:%M:%S")

    # próbujemy pobrać krótki hash gita (jeśli repo istnieje)
    git_hash = ""
    try:
        git_hash = subprocess.check_output(
            ["git", "rev-parse", "--short", "HEAD"],
            cwd=project_dir, stderr=subprocess.DEVNULL
        ).decode().strip()
    except Exception:
        git_hash = ""

    content = []
    content.append("// Auto-generated file. Do not edit.")
    content.append('#ifndef BUILD_INFO_H')
    content.append('#define BUILD_INFO_H')
    content.append('')
    content.append('#define DEVELOPER "{}"'.format(get_build_flag_value("DEVELOPER")))
    content.append('#define BUILD_TIMESTAMP "{}"'.format(timestamp))
    if git_hash:
        content.append('#define BUILD_GITHASH "{}"'.format(git_hash))
    else:
        content.append('// BUILD_GITHASH not available')
    content.append('')
    content.append('#endif // BUILD_INFO_H')
    content = "\n".join(content) + "\n"

    target_file = os.path.join(out_dir, "build_info.h")
    try:
        with open(target_file, "w", encoding="utf-8") as f:
            f.write(content)
        print("Generated", target_file)
        return True
    except Exception as e:
        print("ERROR: nie mogę zapisać pliku:", target_file, e)
        return False

# Funkcja używana jako hook przez PlatformIO:
def generate_build_info(env):
    project_dir = env['PROJECT_DIR']
    print("dupa")
    write_build_info(project_dir)

# 'buildprog' jest dobrym hookiem, uruchamia się przed kompilacją programu
generate_build_info(env)