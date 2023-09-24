#!/usr/bin/env python3

import requests, socket, struct, time, dbus

IP = "10.104.7.163"
MEDIA_PROVIDERS = ["spotify"]
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

class MediaGetter:
    def __init__(self, provider):
        self.dbus = dbus.SessionBus()
        self.media_bus = self.dbus.get_object(f"org.mpris.MediaPlayer2.{provider}", "/org/mpris/MediaPlayer2")
        self.iface = dbus.Interface(self.media_bus, "org.freedesktop.DBus.Properties")

    def get_song(self):
        meta = self.iface.Get("org.mpris.MediaPlayer2.Player", "Metadata")
        pos = int(self.iface.Get("org.mpris.MediaPlayer2.Player", "Position")) / 1000
        artist = meta.get("xesam:albumArtist")
        if artist == None:
            artist = meta.get("xesam:artist")
        if artist != None:
            try:
                artist = str(next(iter(artist)))
            except StopIteration:
                artist = None
        rating = meta.get("xesam:autoRating")
        length = meta.get("mpris:length")
        return (artist,
                str(meta.get("xesam:title")),
                int(length) / 1000 if length != None else None,
                pos,
                float(rating) if rating != None else None)

def get_song():
    song = None
    for prov in MEDIA_PROVIDERS:
        try:
            song = MediaGetter(prov).get_song()
        except dbus.exceptions.DBusException:
            pass
    if song == None:
        return None
    else:
        return song

def post_media_info(track: str, album: str, artist: str, length: str, playing: bool, pos: int):
    data = f"{track}\n{album}\n{artist}\n{length}\n{1 if playing else 0}\n{pos}"
    rsp = requests.post(f"http://{IP}/media", data=bytes(data, "utf-8"))
    assert rsp.status_code == 200

def send_media_state(playing: bool, length: int):
    sock.sendto(bytes([
        1 if playing else 0,
        *struct.pack("<I", length),
    ]), (IP, 1234))

while True:
    song = get_song()
    if song:
        artist, track, length, position, _rating = song
        post_media_info(track, " ", artist, length, True, position)
    else:
        post_media_info(" ", " ", " ", 0, False, 0)
    time.sleep(0.1)
