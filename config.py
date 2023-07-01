import yaml


# Load the config file
def load_config(file):
    with open(file, 'r') as file:
        return yaml.safe_load(file)
