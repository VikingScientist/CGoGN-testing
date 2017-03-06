#ifndef UTIL_IGX_COLORS_H
#define UTIL_IGX_COLORS_H

#include <string>
#include <algorithm>

namespace util
{
	using namespace std;

        inline const char *flashred() { return "\033[0;31;5m"; }
        inline const char *red()      { return "\033[0;31m"; }
        inline const char *green()    { return "\033[0;32m"; }
        inline const char *yellow()   { return "\033[0;33m"; }
        inline const char *blue()     { return "\033[0;34m"; }
        inline const char *magenta()  { return "\033[0;35m"; }
        inline const char *cyang()    { return "\033[0;36m"; }
        inline const char *bred()     { return "\033[0;31;1m"; }
        inline const char *bgreen()   { return "\033[0;32;1m"; }
        inline const char *byellow()  { return "\033[0;33;1m"; }
        inline const char *bblue()    { return "\033[0;34;1m"; }
        inline const char *bmagenta() { return "\033[0;35;1m"; }
        inline const char *bcyang()   { return "\033[0;36;1m"; }
        inline const char *purple()   { return "\033[38;5;91m"; }
        inline const char *orange()   { return "\033[38;5;209m"; }
	inline const char *borange()  { return "\033[38;5;214m"; }
        inline const char *normal()   { return "\033[0m"; }
        inline string c256f( int c )  { return string( "\033[38;5;" ) + to_string( c ) + "m"; }

#if 0
	inline const char *Grey0()             { return "\033[38;5;16m"; }
	inline const char *NavyBlue()          { return "\033[38;5;17m"; }
	inline const char *DarkBlue()          { return "\033[38;5;18m"; }
	inline const char *Blue3()             { return "\033[38;5;19m"; }
	inline const char *Blue3()             { return "\033[38;5;20m"; }
	inline const char *Blue1()             { return "\033[38;5;21m"; }
	inline const char *DarkGreen()         { return "\033[38;5;22m"; }
	inline const char *DeepSkyBlue4()      { return "\033[38;5;23m"; }
	inline const char *DeepSkyBlue4()      { return "\033[38;5;24m"; }
	inline const char *DeepSkyBlue4()      { return "\033[38;5;25m"; }
	inline const char *DodgerBlue3()       { return "\033[38;5;26m"; }
	inline const char *DodgerBlue2()       { return "\033[38;5;27m"; }
	inline const char *Green4()            { return "\033[38;5;28m"; }
	inline const char *SpringGreen4()      { return "\033[38;5;29m"; }
	inline const char *Turquoise4()        { return "\033[38;5;30m"; }
	inline const char *DeepSkyBlue3()      { return "\033[38;5;31m"; }
	inline const char *DeepSkyBlue3()      { return "\033[38;5;32m"; }
	inline const char *DodgerBlue1()       { return "\033[38;5;33m"; }
	inline const char *Green3()            { return "\033[38;5;34m"; }
	inline const char *SpringGreen3()      { return "\033[38;5;35m"; }
	inline const char *DarkCyan()          { return "\033[38;5;36m"; }
	inline const char *LightSeaGreen()     { return "\033[38;5;37m"; }
	inline const char *DeepSkyBlue2()      { return "\033[38;5;38m"; }
	inline const char *DeepSkyBlue1()      { return "\033[38;5;39m"; }
	inline const char *Green3()            { return "\033[38;5;40m"; }
	inline const char *SpringGreen3()      { return "\033[38;5;41m"; }
	inline const char *SpringGreen2()      { return "\033[38;5;42m"; }
	inline const char *Cyan3()             { return "\033[38;5;43m"; }
	inline const char *DarkTurquoise()     { return "\033[38;5;44m"; }
	inline const char *Turquoise2()        { return "\033[38;5;45m"; }
	inline const char *Green1()            { return "\033[38;5;46m"; }
	inline const char *SpringGreen2()      { return "\033[38;5;47m"; }
	inline const char *SpringGreen1()      { return "\033[38;5;48m"; }
	inline const char *MediumSpringGreen() { return "\033[38;5;49m"; }
	inline const char *Cyan2()             { return "\033[38;5;50m"; }
	inline const char *Cyan1()             { return "\033[38;5;51m"; }
	inline const char *DarkRed()           { return "\033[38;5;52m"; }
	inline const char *DeepPink4()         { return "\033[38;5;53m"; }
	inline const char *Purple4()           { return "\033[38;5;54m"; }
	inline const char *Purple4()           { return "\033[38;5;55m"; }
	inline const char *Purple3()           { return "\033[38;5;56m"; }
	inline const char *BlueViolet()        { return "\033[38;5;57m"; }
	inline const char *Orange4()           { return "\033[38;5;58m"; }
	inline const char *Grey37()            { return "\033[38;5;59m"; }
	inline const char *MediumPurple4()     { return "\033[38;5;60m"; }
	inline const char *SlateBlue3()        { return "\033[38;5;61m"; }
	inline const char *SlateBlue3()        { return "\033[38;5;62m"; }
	inline const char *RoyalBlue1()        { return "\033[38;5;63m"; }
	inline const char *Chartreuse4()       { return "\033[38;5;64m"; }
	inline const char *DarkSeaGreen4()     { return "\033[38;5;65m"; }
	inline const char *PaleTurquoise4()    { return "\033[38;5;66m"; }
	inline const char *SteelBlue()         { return "\033[38;5;67m"; }
	inline const char *SteelBlue3()        { return "\033[38;5;68m"; }
	inline const char *CornflowerBlue()    { return "\033[38;5;69m"; }
	inline const char *Chartreuse3()       { return "\033[38;5;70m"; }
	inline const char *DarkSeaGreen4()     { return "\033[38;5;71m"; }
	inline const char *CadetBlue()         { return "\033[38;5;72m"; }
	inline const char *CadetBlue()         { return "\033[38;5;73m"; }
	inline const char *SkyBlue3()          { return "\033[38;5;74m"; }
	inline const char *SteelBlue1()        { return "\033[38;5;75m"; }
	inline const char *Chartreuse3()       { return "\033[38;5;76m"; }
	inline const char *PaleGreen3()        { return "\033[38;5;77m"; }
	inline const char *SeaGreen3()         { return "\033[38;5;78m"; }
	inline const char *Aquamarine3()       { return "\033[38;5;79m"; }
	inline const char *MediumTurquoise()   { return "\033[38;5;80m"; }
	inline const char *SteelBlue1()        { return "\033[38;5;81m"; }
	inline const char *Chartreuse2()       { return "\033[38;5;82m"; }
	inline const char *SeaGreen2()         { return "\033[38;5;83m"; }
	inline const char *SeaGreen1()         { return "\033[38;5;84m"; }
	inline const char *SeaGreen1()         { return "\033[38;5;85m"; }
	inline const char *Aquamarine1()       { return "\033[38;5;86m"; }
	inline const char *DarkSlateGray2()    { return "\033[38;5;87m"; }
	inline const char *DarkRed()           { return "\033[38;5;88m"; }
	inline const char *DeepPink4()         { return "\033[38;5;89m"; }
	inline const char *DarkMagenta()       { return "\033[38;5;90m"; }
	inline const char *DarkMagenta()       { return "\033[38;5;91m"; }
	inline const char *DarkViolet()        { return "\033[38;5;92m"; }
	inline const char *Purple()            { return "\033[38;5;93m"; }
	inline const char *Orange4()           { return "\033[38;5;94m"; }
	inline const char *LightPink4()        { return "\033[38;5;95m"; }
	inline const char *Plum4()             { return "\033[38;5;96m"; }
	inline const char *MediumPurple3()     { return "\033[38;5;97m"; }
	inline const char *MediumPurple3()     { return "\033[38;5;98m"; }
	inline const char *SlateBlue1()        { return "\033[38;5;99m"; }
	inline const char *Yellow4()           { return "\033[38;5;100m"; }
	inline const char *Wheat4()            { return "\033[38;5;101m"; }
	inline const char *Grey53()            { return "\033[38;5;102m"; }
	inline const char *LightSlateGrey()    { return "\033[38;5;103m"; }
	inline const char *MediumPurple()      { return "\033[38;5;104m"; }
	inline const char *LightSlateBlue()    { return "\033[38;5;105m"; }
	inline const char *Yellow4()           { return "\033[38;5;106m"; }
	inline const char *DarkOliveGreen3()   { return "\033[38;5;107m"; }
	inline const char *DarkSeaGreen()      { return "\033[38;5;108m"; }
	inline const char *LightSkyBlue3()     { return "\033[38;5;109m"; }
	inline const char *LightSkyBlue3()     { return "\033[38;5;110m"; }
	inline const char *SkyBlue2()          { return "\033[38;5;111m"; }
	inline const char *Chartreuse2()       { return "\033[38;5;112m"; }
	inline const char *DarkOliveGreen3()   { return "\033[38;5;113m"; }
	inline const char *PaleGreen3()        { return "\033[38;5;114m"; }
	inline const char *DarkSeaGreen3()     { return "\033[38;5;115m"; }
	inline const char *DarkSlateGray3()    { return "\033[38;5;116m"; }
	inline const char *SkyBlue1()          { return "\033[38;5;117m"; }
	inline const char *Chartreuse1()       { return "\033[38;5;118m"; }
	inline const char *LightGreen()        { return "\033[38;5;119m"; }
	inline const char *LightGreen()        { return "\033[38;5;120m"; }
	inline const char *PaleGreen1()        { return "\033[38;5;121m"; }
	inline const char *Aquamarine1()       { return "\033[38;5;122m"; }
	inline const char *DarkSlateGray1()    { return "\033[38;5;123m"; }
	inline const char *Red3()              { return "\033[38;5;124m"; }
	inline const char *DeepPink4()         { return "\033[38;5;125m"; }
	inline const char *MediumVioletRed()   { return "\033[38;5;126m"; }
	inline const char *Magenta3()          { return "\033[38;5;127m"; }
	inline const char *DarkViolet()        { return "\033[38;5;128m"; }
	inline const char *Purple()            { return "\033[38;5;129m"; }
	inline const char *DarkOrange3()       { return "\033[38;5;130m"; }
	inline const char *IndianRed()         { return "\033[38;5;131m"; }
	inline const char *HotPink3()          { return "\033[38;5;132m"; }
	inline const char *MediumOrchid3()     { return "\033[38;5;133m"; }
	inline const char *MediumOrchid()      { return "\033[38;5;134m"; }
	inline const char *MediumPurple2()     { return "\033[38;5;135m"; }
	inline const char *DarkGoldenrod()     { return "\033[38;5;136m"; }
	inline const char *LightSalmon3()      { return "\033[38;5;137m"; }
	inline const char *RosyBrown()         { return "\033[38;5;138m"; }
	inline const char *Grey63()            { return "\033[38;5;139m"; }
	inline const char *MediumPurple2()     { return "\033[38;5;140m"; }
	inline const char *MediumPurple1()     { return "\033[38;5;141m"; }
	inline const char *Gold3()             { return "\033[38;5;142m"; }
	inline const char *DarkKhaki()         { return "\033[38;5;143m"; }
	inline const char *NavajoWhite3()      { return "\033[38;5;144m"; }
	inline const char *Grey69()            { return "\033[38;5;145m"; }
	inline const char *LightSteelBlue3()   { return "\033[38;5;146m"; }
	inline const char *LightSteelBlue()    { return "\033[38;5;147m"; }
	inline const char *Yellow3()           { return "\033[38;5;148m"; }
	inline const char *DarkOliveGreen3()   { return "\033[38;5;149m"; }
	inline const char *DarkSeaGreen3()     { return "\033[38;5;150m"; }
	inline const char *DarkSeaGreen2()     { return "\033[38;5;151m"; }
	inline const char *LightCyan3()        { return "\033[38;5;152m"; }
	inline const char *LightSkyBlue1()     { return "\033[38;5;153m"; }
	inline const char *GreenYellow()       { return "\033[38;5;154m"; }
	inline const char *DarkOliveGreen2()   { return "\033[38;5;155m"; }
	inline const char *PaleGreen1()        { return "\033[38;5;156m"; }
	inline const char *DarkSeaGreen2()     { return "\033[38;5;157m"; }
	inline const char *DarkSeaGreen1()     { return "\033[38;5;158m"; }
	inline const char *PaleTurquoise1()    { return "\033[38;5;159m"; }
	inline const char *Red3()              { return "\033[38;5;160m"; }
	inline const char *DeepPink3()         { return "\033[38;5;161m"; }
	inline const char *DeepPink3()         { return "\033[38;5;162m"; }
	inline const char *Magenta3()          { return "\033[38;5;163m"; }
	inline const char *Magenta3()          { return "\033[38;5;164m"; }
	inline const char *Magenta2()          { return "\033[38;5;165m"; }
	inline const char *DarkOrange3()       { return "\033[38;5;166m"; }
	inline const char *IndianRed()         { return "\033[38;5;167m"; }
	inline const char *HotPink3()          { return "\033[38;5;168m"; }
	inline const char *HotPink2()          { return "\033[38;5;169m"; }
	inline const char *Orchid()            { return "\033[38;5;170m"; }
	inline const char *MediumOrchid1()     { return "\033[38;5;171m"; }
	inline const char *Orange3()           { return "\033[38;5;172m"; }
	inline const char *LightSalmon3()      { return "\033[38;5;173m"; }
	inline const char *LightPink3()        { return "\033[38;5;174m"; }
	inline const char *Pink3()             { return "\033[38;5;175m"; }
	inline const char *Plum3()             { return "\033[38;5;176m"; }
	inline const char *Violet()            { return "\033[38;5;177m"; }
	inline const char *Gold3()             { return "\033[38;5;178m"; }
	inline const char *LightGoldenrod3()   { return "\033[38;5;179m"; }
	inline const char *Tan()               { return "\033[38;5;180m"; }
	inline const char *MistyRose3()        { return "\033[38;5;181m"; }
	inline const char *Thistle3()          { return "\033[38;5;182m"; }
	inline const char *Plum2()             { return "\033[38;5;183m"; }
	inline const char *Yellow3()           { return "\033[38;5;184m"; }
	inline const char *Khaki3()            { return "\033[38;5;185m"; }
	inline const char *LightGoldenrod2()   { return "\033[38;5;186m"; }
	inline const char *LightYellow3()      { return "\033[38;5;187m"; }
	inline const char *Grey84()            { return "\033[38;5;188m"; }
	inline const char *LightSteelBlue1()   { return "\033[38;5;189m"; }
	inline const char *Yellow2()           { return "\033[38;5;190m"; }
	inline const char *DarkOliveGreen1()   { return "\033[38;5;191m"; }
	inline const char *DarkOliveGreen1()   { return "\033[38;5;192m"; }
	inline const char *DarkSeaGreen1()     { return "\033[38;5;193m"; }
	inline const char *Honeydew2()         { return "\033[38;5;194m"; }
	inline const char *LightCyan1()        { return "\033[38;5;195m"; }
	inline const char *Red1()              { return "\033[38;5;196m"; }
	inline const char *DeepPink2()         { return "\033[38;5;197m"; }
	inline const char *DeepPink1()         { return "\033[38;5;198m"; }
	inline const char *DeepPink1()         { return "\033[38;5;199m"; }
	inline const char *Magenta2()          { return "\033[38;5;200m"; }
	inline const char *Magenta1()          { return "\033[38;5;201m"; }
	inline const char *OrangeRed1()        { return "\033[38;5;202m"; }
	inline const char *IndianRed1()        { return "\033[38;5;203m"; }
	inline const char *IndianRed1()        { return "\033[38;5;204m"; }
	inline const char *HotPink()           { return "\033[38;5;205m"; }
	inline const char *HotPink()           { return "\033[38;5;206m"; }
	inline const char *MediumOrchid1()     { return "\033[38;5;207m"; }
	inline const char *DarkOrange()        { return "\033[38;5;208m"; }
	inline const char *Salmon1()           { return "\033[38;5;209m"; }
	inline const char *LightCoral()        { return "\033[38;5;210m"; }
	inline const char *PaleVioletRed1()    { return "\033[38;5;211m"; }
	inline const char *Orchid2()           { return "\033[38;5;212m"; }
	inline const char *Orchid1()           { return "\033[38;5;213m"; }
	inline const char *Orange1()           { return "\033[38;5;214m"; }
	inline const char *SandyBrown()        { return "\033[38;5;215m"; }
	inline const char *LightSalmon1()      { return "\033[38;5;216m"; }
	inline const char *LightPink1()        { return "\033[38;5;217m"; }
	inline const char *Pink1()             { return "\033[38;5;218m"; }
	inline const char *Plum1()             { return "\033[38;5;219m"; }
	inline const char *Gold1()             { return "\033[38;5;220m"; }
	inline const char *LightGoldenrod2()   { return "\033[38;5;221m"; }
	inline const char *LightGoldenrod2()   { return "\033[38;5;222m"; }
	inline const char *NavajoWhite1()      { return "\033[38;5;223m"; }
	inline const char *MistyRose1()        { return "\033[38;5;224m"; }
	inline const char *Thistle1()          { return "\033[38;5;225m"; }
	inline const char *Yellow1()           { return "\033[38;5;226m"; }
	inline const char *LightGoldenrod1()   { return "\033[38;5;227m"; }
	inline const char *Khaki1()            { return "\033[38;5;228m"; }
	inline const char *Wheat1()            { return "\033[38;5;229m"; }
	inline const char *Cornsilk1()         { return "\033[38;5;230m"; }
	inline const char *Grey100()           { return "\033[38;5;231m"; }
	inline const char *Grey3()             { return "\033[38;5;232m"; }
	inline const char *Grey7()             { return "\033[38;5;233m"; }
	inline const char *Grey11()            { return "\033[38;5;234m"; }
	inline const char *Grey15()            { return "\033[38;5;235m"; }
	inline const char *Grey19()            { return "\033[38;5;236m"; }
	inline const char *Grey23()            { return "\033[38;5;237m"; }
	inline const char *Grey27()            { return "\033[38;5;238m"; }
	inline const char *Grey30()            { return "\033[38;5;239m"; }
	inline const char *Grey35()            { return "\033[38;5;240m"; }
	inline const char *Grey39()            { return "\033[38;5;241m"; }
	inline const char *Grey42()            { return "\033[38;5;242m"; }
	inline const char *Grey46()            { return "\033[38;5;243m"; }
	inline const char *Grey50()            { return "\033[38;5;244m"; }
	inline const char *Grey54()            { return "\033[38;5;245m"; }
	inline const char *Grey58()            { return "\033[38;5;246m"; }
	inline const char *Grey62()            { return "\033[38;5;247m"; }
	inline const char *Grey66()            { return "\033[38;5;248m"; }
	inline const char *Grey70()            { return "\033[38;5;249m"; }
	inline const char *Grey74()            { return "\033[38;5;250m"; }
	inline const char *Grey78()            { return "\033[38;5;251m"; }
	inline const char *Grey82()            { return "\033[38;5;252m"; }
	inline const char *Grey85()            { return "\033[38;5;253m"; }
	inline const char *Grey89()            { return "\033[38;5;254m"; }
	inline const char *Grey93()            { return "\033[38;5;255m"; }
#endif
	inline ostream& resetl( ostream& o ) {
		o << normal() << endl;
		return o;
	}
	inline ostream& reset( ostream& o ) {
		o << normal();
		return o;
	}


	inline bool endsWith( const std::string& value, const std::string& ending )
	{
		if (ending.size() > value.size()) return false;
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}

	inline string toLower( const string& s )
	{
		string ret;
		ret.resize( s.length() );
		std::transform( s.begin(), s.end(), ret.begin(), ::tolower );
		return ret;
	}
}
#endif
