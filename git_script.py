import subprocess
import os


# Crea il file git_revision.h
f = open("include/git_revision.h", "w")

local_path = os.path.abspath(".")
local_path = local_path.replace("\\", "/")

# Controlla se la cartella corrente è un repository git
try:
    subprocess.check_output(["git", "rev-parse", "HEAD"])
    is_git_repo = True
except subprocess.CalledProcessError:
    is_git_repo = False

# Se è un repository git, prendi l'ID del commit più recente
if is_git_repo:
    log_command = ["git", "log", "--oneline"]
    try:
        output = subprocess.check_output(log_command, universal_newlines=True)
        commits = output.strip().split('\n')

        if commits:
            # Se ci sono commit, prendi l'ID del commit più recente
            latest_commit_id = commits[0].split()[0]

            # Salva l'ID del commit su un file
            f.write("#ifndef __GIT_COMMIT__\n")
            f.write("#define __GIT_COMMIT__ \"%s\"\n" % latest_commit_id)
            f.write("#endif\n")

            # Verifica se esiste una repository remota
            remote_url_command = ["git", "config", "--get", "remote.origin.url"]
            try:
                remote_url = subprocess.check_output(remote_url_command, universal_newlines=True).strip()
                f.write("#ifndef __GIT_REMOTE_URL__\n")
                f.write("#define __GIT_REMOTE_URL__ \"%s\"\n" % remote_url)
                f.write("#endif\n")
            except subprocess.CalledProcessError:
                f.write("#ifndef __GIT_REMOTE_URL__\n")
                f.write("#define __GIT_REMOTE_URL__ \"L:%s\"\n" % local_path)
                f.write("#endif\n")

        else:
            f.write("#ifndef __GIT_COMMIT__\n")
            f.write("#define __GIT_COMMIT__ \"NO_COMMITS_YET\"\n")
            f.write("#endif\n")
            # Remote URL
            f.write("#ifndef __GIT_REMOTE_URL__\n")
            f.write("#define __GIT_REMOTE_URL__ \"L:%s\"\n" % local_path)
            f.write("#endif\n")
    except subprocess.CalledProcessError:
        f.write("#ifndef __GIT_COMMIT__\n")
        f.write("#define __GIT_COMMIT__ \"SCRIPT_ERROR\"\n")
        f.write("#endif\n")
        # Remote URL
        f.write("#ifndef __GIT_REMOTE_URL__\n")
        f.write("#define __GIT_REMOTE_URL__ \"L:%s\"\n" % local_path)
        f.write("#endif\n")
else:
    f.write("#ifndef __GIT_COMMIT__\n")
    f.write("#define __GIT_COMMIT__ \"NO_REPO\"\n")
    f.write("#endif\n")
    # Remote URL
    f.write("#ifndef __GIT_REMOTE_URL__\n")
    f.write("#define __GIT_REMOTE_URL__ \"L:%s\"\n" % local_path)
    f.write("#endif\n")

# Chiudi il file
f.close()

# .gitignore update
gitignore = open(".gitignore", "r")
if "git_revision.h" not in gitignore.read():
    gitignore.close()
    gitignore = open(".gitignore", "a")
    gitignore.write("git_revision.h\n")
    gitignore.close()
