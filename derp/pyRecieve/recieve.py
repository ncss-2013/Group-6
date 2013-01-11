import serial
import simplekml

lstyle = simplekml.LineStyle()
lstyle.color = '7f00ffff'
lstyle.width = 4
pstyle = simplekml.PolyStyle(0, 1)
style = simplekml.Style(linestyle=lstyle, polystyle=pstyle)

kml = simplekml.Kml()
ls = kml.newlinestring(name='Herp')
ls.coords = [
	(-112.2550785337791,36.07954952145647,2357),
	(-112.2549277039738,36.08117083492122,2357),
	(-112.2552505069063,36.08260761307279,2357),
	(-112.2564540158376,36.08395660588506,2357),
	(-112.2580238976449,36.08511401044813,2357),
	(-112.2595218489022,36.08584355239394,2357),
	(-112.2608216347552,36.08612634548589,2357),
	(-112.262073428656,36.08626019085147,2357),
	(-112.2633204928495,36.08621519860091,2357),
	(-112.2644963846444,36.08627897945274,2357),
	(-112.2656969554589,36.08649599090644,2357),
]
ls.extrude = 1
ls.altitudemode = simplekml.AltitudeMode.absolute
ls.style = style
kml.save('Test2.kml')