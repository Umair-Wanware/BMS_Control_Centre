Import("env")
import os
import shutil

PROJECT_DIR = env.subst("$PROJECT_DIR")
WEB_DIR = os.path.join(PROJECT_DIR, "web")
DATA_DIR = os.path.join(PROJECT_DIR, "data")


def sync_web_assets(source, target, env):
    if not os.path.isdir(WEB_DIR):
        print("sync_web_to_data: web/ directory not found, skipping")
        return

    os.makedirs(DATA_DIR, exist_ok=True)

    for entry in os.listdir(WEB_DIR):
        source_path = os.path.join(WEB_DIR, entry)
        if not os.path.isfile(source_path):
            continue
        shutil.copy2(source_path, os.path.join(DATA_DIR, entry))

    print(f"sync_web_to_data: copied web assets into {DATA_DIR}")


env.AddPreAction("buildfs", sync_web_assets)
