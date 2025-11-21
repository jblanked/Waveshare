class SD:
    """Class to interact with SD card."""

    def __init__(self):
        from waveshare_sd import init

        init()

    @property
    def active(self):
        from waveshare_sd import is_initialized

        return is_initialized()

    def create_file(self, path):
        from waveshare_sd import create_file

        return create_file(path)

    def create_directory(self, path):
        from waveshare_sd import create_directory

        return create_directory(path)

    def exists(self, path):
        from waveshare_sd import exists

        return exists(path)

    def get_file_size(self, path):
        from waveshare_sd import get_file_size

        return get_file_size(path)

    def get_free_space(self):
        from waveshare_sd import get_free_space

        return get_free_space()

    def get_total_space(self):
        from waveshare_sd import get_total_space

        return get_total_space()

    def is_directory(self, path):
        from waveshare_sd import is_directory

        return is_directory(path)

    def is_file(self, path):
        from waveshare_sd import is_file

        return is_file(path)

    def read(self, path, index=0, count=-1):
        from waveshare_sd import read

        return read(path, index, count)

    def remove(self, path):
        from waveshare_sd import remove

        return remove(path)

    def rename(self, old_path, new_path):
        from waveshare_sd import rename

        return rename(old_path, new_path)

    def write(self, path, data, overwrite=True):
        from waveshare_sd import write

        return write(path, data, overwrite)
