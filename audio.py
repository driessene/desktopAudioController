from pycaw.pycaw import AudioUtilities


# Get the available applications for audio manipulation
def available_apps():
    sessions = AudioUtilities.GetAllSessions()
    app_names = []
    for session in sessions:
        if session.Process:
            app_names.append(session.Process.name())
    return app_names


# Set the app volume
def set_app_volume(app_name, volume):
    sessions = AudioUtilities.GetAllSessions()
    for session in sessions:
        volume_ctrl = session.SimpleAudioVolume
        if session.Process and session.Process.name() == app_name:
            volume_ctrl.SetMasterVolume(volume, None)
