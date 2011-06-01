; EAGLE Autorouter Control File

[Default]

  RoutingGrid     = 1mm

  ; Trace Parameters:

  tpViaShape      = Octagon

  ; Preferred Directions:

  PrefDir.1       = -
  PrefDir.2       = 0
  PrefDir.3       = 0
  PrefDir.4       = 0
  PrefDir.5       = 0
  PrefDir.6       = 0
  PrefDir.7       = 0
  PrefDir.8       = 0
  PrefDir.9       = 0
  PrefDir.10      = 0
  PrefDir.11      = 0
  PrefDir.12      = 0
  PrefDir.13      = 0
  PrefDir.14      = 0
  PrefDir.15      = 0
  PrefDir.16      = |

  Active          =    1
  ; Cost Factors:

  cfVia           =    8
  cfNonPref       =    5
  cfChangeDir     =    2
  cfOrthStep      =    2
  cfDiagStep      =    3
  cfExtdStep      =    0
  cfBonusStep     =    1
  cfMalusStep     =    1
  cfPadImpact     =    4
  cfSmdImpact     =    4
  cfBusImpact     =    0
  cfHugging       =    3
  cfAvoid         =    4
  cfPolygon       =   10

  cfBase.1        =    0
  cfBase.2        =    1
  cfBase.3        =    1
  cfBase.4        =    1
  cfBase.5        =    1
  cfBase.6        =    1
  cfBase.7        =    1
  cfBase.8        =    1
  cfBase.9        =    1
  cfBase.10       =    1
  cfBase.11       =    1
  cfBase.12       =    1
  cfBase.13       =    1
  cfBase.14       =    1
  cfBase.15       =    1
  cfBase.16       =    0

  ; Maximum Number of...:

  mnVias          =   20
  mnSegments      = 9999
  mnExtdSteps     = 9999
  mnRipupLevel    =   10
  mnRipupSteps    =  100
  mnRipupTotal    =  100

[Follow-me]

  @Route

  Active          =    1
  cfVia           =   20
  cfNonPref       =    3
  cfChangeDir     =    5

[Busses]

  @Route

  Active          =    1
  cfNonPref       =    4
  cfBusImpact     =    4
  cfHugging       =    0
  mnVias          =    0

[Route]

  @Default

  Active          =    1

[Optimize1]

  @Route

  Active          =    1
  cfVia           =   99
  cfExtdStep      =   10
  cfHugging       =    1
  mnExtdSteps     =    1
  mnRipupLevel    =    0

[Optimize2]

  @Optimize1

  Active          =    1
  cfNonPref       =    0
  cfChangeDir     =    6
  cfExtdStep      =    0
  cfBonusStep     =    2
  cfMalusStep     =    2
  cfPadImpact     =    2
  cfSmdImpact     =    2
  cfHugging       =    0

[Optimize3]

  @Optimize2

  Active          =    1
  cfChangeDir     =    8
  cfPadImpact     =    0
  cfSmdImpact     =    0

[Optimize4]

  @Optimize3

  Active          =    1
  cfChangeDir     =   25

