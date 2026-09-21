<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>XFCE Hardware Vision Word-Compiler</title>
    <!-- Tailwind CSS (Permitted CDN) -->
    <script src="https://cdn.tailwindcss.com"></script>
    <link href="https://fonts.googleapis.com/css2?family=Fira+Code:wght@400;500;600&family=Inter:wght@300;400;500;600;700&display=swap" rel="stylesheet">
    <style>
        body {
            font-family: 'Inter', sans-serif;
            touch-action: manipulation;
            -webkit-tap-highlight-color: transparent;
        }
        .terminal-font {
            font-family: 'Fira Code', monospace;
        }
        /* Custom scrollbars inspired by classic XFCE themes */
        ::-webkit-scrollbar {
            width: 6px;
            height: 6px;
        }
        ::-webkit-scrollbar-track {
            background: #262626;
        }
        ::-webkit-scrollbar-thumb {
            background: #404040;
            border-radius: 3px;
        }
        ::-webkit-scrollbar-thumb:hover {
            background: #525252;
        }
        @keyframes blink {
            0%, 100% { opacity: 1; }
            50% { opacity: 0; }
        }
        .cursor-blink {
            animation: blink 1s infinite;
        }
        /* XFCE window shadows and borders */
        .xfce-window {
            box-shadow: 0 10px 25px -5px rgba(0, 0, 0, 0.5), 0 8px 10px -6px rgba(0, 0, 0, 0.5);
            border: 1px solid #404040;
        }
        /* Specialized container heights for virtual scrolling on small touchscreens */
        .mobile-height-calc {
            height: calc(100vh - 120px);
        }
        @media (min-width: 1280px) {
            .mobile-height-calc {
                height: calc(100vh - 65px);
            }
        }
    </style>
</head>
<body class="bg-zinc-800 text-zinc-100 min-h-screen flex flex-col selection:bg-blue-500/40 select-none overflow-hidden fixed inset-0 w-full home-viewport-fix">

    <!-- XFCE Top Desktop Panel (Touch Optimized Height) -->
    <div class="h-9 bg-zinc-900 border-b border-zinc-950 px-2 flex items-center justify-between text-xs font-medium text-zinc-300 z-50 shrink-0 select-none">
        <div class="flex items-center gap-2">
            <!-- XFCE Application Menu Button -->
            <button class="bg-zinc-800 hover:bg-zinc-700 active:bg-zinc-900 border border-zinc-700 px-3 py-1 rounded flex items-center gap-1.5 transition text-zinc-200 font-semibold min-h-[28px]">
                <svg class="w-3.5 h-3.5 text-blue-400" viewBox="0 0 24 24" fill="currentColor">
                    <path d="M12 2C7.58 2 4 5.58 4 10v4c0 4.42 3.58 8 8 8s8-3.58 8-8v-4c0-4.42-3.58-8-8-8zm-1 3c0-.55.45-1 1-1s1 .45 1 1v3c0 .55-.45 1-1 1s-1-.45-1-1V5zm5 9c0 .55-.45 1-1 1H9c-.55 0-1-.45-1-1s.45-1 1-1h7c0 .55 0 1 1 1z"/>
                </svg>
                <span class="hidden sm:inline">Applications</span>
            </button>
            <div class="h-4 w-[1px] bg-zinc-700 mx-1"></div>
            <span id="desktopWorkspaceLabel" class="text-zinc-400 text-[11px] font-mono truncate max-w-[140px] sm:max-w-none">Workspace: /visions</span>
        </div>
        
        <!-- XFCE Window Display Mode Switcher (Visible on Mobile Viewports for Display Switching) -->
        <div class="xl:hidden flex items-center bg-zinc-950 p-0.5 rounded border border-zinc-800 font-mono text-[10px]">
            <button id="viewTabThunar" onclick="switchMobileWindow('thunar')" class="px-2.5 py-1 rounded-sm text-blue-400 bg-zinc-800 font-bold transition-all">Thunar</button>
            <button id="viewTabTerminal" onclick="switchMobileWindow('terminal')" class="px-2.5 py-1 rounded-sm text-zinc-400 transition-all">Terminal</button>
        </div>
        
        <div class="hidden sm:flex items-center gap-3 text-[11px] font-mono text-zinc-400">
            <!-- Web Serial Interface Connection Status Node Toggle -->
            <button id="hwConnectionBadge" onclick="toggleHardwarePort()" class="bg-zinc-950 hover:bg-zinc-900 text-amber-500 border border-zinc-800 px-2 py-0.5 rounded text-[10px] font-bold flex items-center gap-1 transition-colors">
                <span class="w-1.5 h-1.5 rounded-full bg-amber-500" id="hwStatusDot"></span>
                <span id="hwStatusText">Arduino: Disconnected</span>
            </button>
            <div class="h-4 w-[1px] bg-zinc-700 mx-1"></div>
            <span>Mon Sep 21, 12:25 PM</span>
        </div>
    </div>

    <!-- Main Desktop Backdrop Environment Area -->
    <main class="flex-1 p-2 sm:p-3 overflow-hidden bg-gradient-to-br from-zinc-900 via-neutral-900 to-zinc-900 relative mobile-height-calc">
        
        <!-- Background Wallpaper Silhouette Motif -->
        <div class="absolute inset-0 flex items-center justify-center pointer-events-none opacity-[0.02] text-zinc-100">
            <svg class="w-72 h-72 sm:w-96 sm:h-96" fill="currentColor" viewBox="0 0 24 24">
                <path d="M12 2C7.58 2 4 5.58 4 10v4c0 4.42 3.58 8 8 8s8-3.58 8-8v-4c0-4.42-3.58-8-8-8z"/>
            </svg>
        </div>

        <div class="w-full h-full grid grid-cols-1 xl:grid-cols-12 gap-2 sm:gap-3 relative overflow-hidden">
            
            <!-- Left Window: Thunar File Manager Window -->
            <section id="windowThunar" class="xl:col-span-4 flex flex-col xfce-window bg-zinc-900 rounded overflow-hidden h-full z-10 transition-all duration-200 absolute inset-0 xl:relative xl:opacity-100 xl:pointer-events-auto">
                <!-- Thunar Window Title Bar -->
                <div class="h-8 bg-gradient-to-b from-zinc-700 to-zinc-800 border-b border-zinc-950 px-2 flex items-center justify-between shrink-0">
                    <div class="flex items-center gap-2">
                        <svg class="w-4 h-4 text-amber-400" fill="currentColor" viewBox="0 0 20 20">
                            <path fill-rule="evenodd" d="M2 6a2 2 0 012-2h4l2 2h4a2 2 0 012 2v5a2 2 0 01-2 2H4a2 2 0 01-2-2V6z" clip-rule="evenodd"/>
                        </svg>
                        <span class="text-xs font-semibold text-zinc-200">visions - Thunar File Manager</span>
                    </div>
                    <div class="flex items-center gap-1">
                        <button class="w-5 h-5 bg-zinc-800 hover:bg-zinc-700 text-zinc-400 flex items-center justify-center rounded-sm text-xs border border-zinc-950 font-mono">-</button>
                        <button class="w-5 h-5 bg-zinc-800 hover:bg-zinc-700 text-zinc-400 flex items-center justify-center rounded-sm text-[10px] border border-zinc-950 font-mono">□</button>
                        <button class="w-5 h-5 bg-zinc-800 hover:bg-rose-600 hover:text-white text-zinc-400 flex items-center justify-center rounded-sm text-xs border border-zinc-950 font-mono">✕</button>
                    </div>
                </div>

                <!-- Thunar Menu Options Bar Layout -->
                <div class="h-6 bg-zinc-800 border-b border-zinc-950 px-2 flex items-center gap-3.5 text-[11px] text-zinc-400 shrink-0">
                    <span class="hover:text-zinc-200 cursor-pointer">File</span>
                    <span class="hover:text-zinc-200 cursor-pointer">Edit</span>
                    <span class="hover:text-zinc-200 cursor-pointer">View</span>
                    <span class="hover:text-zinc-200 cursor-pointer">Go</span>
                </div>

                <!-- Thunar Path Address Bar Location Tracker -->
                <div class="h-7 bg-zinc-950 border-b border-zinc-900 px-2 flex items-center gap-1 text-[11px] shrink-0 font-mono text-zinc-400">
                    <span class="text-zinc-600">Path:</span>
                    <span id="thunarPathLabel" class="text-blue-400">/visions</span>
                </div>

                <!-- Thunar Main Split Content Frame View -->
                <div class="flex-1 flex overflow-hidden">
                    <!-- Thunar Places Side Bar -->
                    <div class="w-24 sm:w-28 bg-zinc-950/40 border-r border-zinc-950 p-1.5 text-[11px] text-zinc-400 font-medium space-y-2.5 shrink-0 overflow-y-auto">
                        <div class="text-zinc-600 font-bold tracking-wider text-[9px] uppercase px-1">Devices</div>
                        <div class="space-y-1 pl-0.5">
                            <button id="sidebar-target-fs" onclick="navigateVirtualFolder('file_system')" class="w-full text-left flex items-center gap-1.5 text-zinc-400 hover:text-zinc-200 py-1.5 px-1 rounded transition-colors min-h-[32px]">
                                <span class="text-zinc-500 text-xs">🖴</span> File System
                            </button>
                        </div>
                        <div class="text-zinc-600 font-bold tracking-wider text-[9px] uppercase pt-1 px-1">Places</div>
                        <div class="space-y-1 pl-0.5">
                            <button id="sidebar-target-home" onclick="navigateVirtualFolder('home')" class="w-full text-left flex items-center gap-1.5 text-zinc-400 hover:text-zinc-200 py-1.5 px-1 rounded transition-colors min-h-[32px]">
                                <span class="text-xs">🏠</span> Home
                            </button>
                            <button id="sidebar-target-desktop" onclick="navigateVirtualFolder('desktop')" class="w-full text-left flex items-center gap-1.5 text-zinc-400 hover:text-zinc-200 py-1.5 px-1 rounded transition-colors min-h-[32px]">
                                <span class="text-xs">🖥</span> Desktop
                            </button>
                            <button id="sidebar-target-visions" onclick="navigateVirtualFolder('visions')" class="w-full text-left flex items-center gap-1.5 text-blue-400 bg-zinc-800/60 px-1 py-1 rounded-sm min-h-[32px]">
                                <span class="text-xs">📂</span> visions
                            </button>
                        </div>
                    </div>

                    <!-- Thunar Folder Contents Grid Frame -->
                    <div class="flex-1 bg-zinc-900/60 p-2 sm:p-3 overflow-y-auto flex flex-col justify-between">
                        <div class="space-y-3">
                            <div id="dirCategoryHeader" class="text-zinc-500 text-[10px] font-mono tracking-wider uppercase border-b border-zinc-800 pb-1">Concept Seed Directory Files</div>
                            
                            <!-- Dynamic File Storage Content Container Frame Grid -->
                            <div id="fileContentGrid" class="grid grid-cols-1 gap-1.5">
                                <!-- Populated dynamically via JS execution routines -->
                            </div>
                        </div>

                        <!-- Target Configuration Subframe -->
                        <div class="border-t border-zinc-800 pt-3 space-y-3 mt-4">
                            <div class="text-zinc-500 text-[10px] font-mono tracking-wider uppercase">Lowering Controls</div>
                            <div>
                                <label class="block text-[10px] font-mono text-zinc-400 mb-1">Target Compilation Type</label>
                                <select id="targetCompilationFormat" class="w-full bg-zinc-950 border border-zinc-800 rounded px-2 py-2 text-xs text-zinc-300 font-mono focus:outline-none focus:border-blue-500 min-h-[36px]">
                                    <option value="narrative">Cinematic Immersive Prose (.prose)</option>
                                    <option value="blueprint">Detailed World-Building Architecture (.spec)</option>
                                    <option value="poetic">Evocative Imagery Lore Stanzas (.verse)</option>
                                    <option value="arduino">Arduino C++ Hardware Sketch (.ino)</option>
                                </select>
                            </div>
                            <div class="bg-zinc-950/60 p-2 rounded border border-zinc-950 text-[10px] font-mono text-zinc-500 space-y-1">
                                <div class="flex justify-between"><span>Selected File:</span><span id="activeFileLabel" class="text-zinc-300 truncate max-w-[100px] sm:max-w-[120px]">None</span></div>
                                <div class="flex justify-between"><span>Output Rule:</span><span class="text-purple-400">Read/Write Cache</span></div>
                            </div>
                        </div>
                    </div>
                </div>
            </section>

            <!-- Right Window: XFCE Terminal Frame & Write Target Shell -->
            <section id="windowTerminal" class="xl:col-span-8 flex flex-col gap-2 sm:gap-3 overflow-hidden h-full z-20 absolute inset-0 xl:relative opacity-0 pointer-events-none xl:opacity-100 xl:pointer-events-auto">
                
                <!-- XFCE Terminal Window App container -->
                <div class="flex-1 flex flex-col xfce-window bg-zinc-950 rounded overflow-hidden min-h-[180px]">
                    <!-- XFCE Terminal App Header Bar -->
                    <div class="h-8 bg-gradient-to-b from-zinc-700 to-zinc-800 border-b border-zinc-950 px-2 flex items-center justify-between shrink-0">
                        <div class="flex items-center gap-2">
                            <span class="text-zinc-300 font-bold text-xs">$_</span>
                            <span id="terminalTitleLabel" class="text-xs font-semibold text-zinc-200 font-mono truncate max-w-[180px] sm:max-w-none">aethon@imagination: /visions</span>
                        </div>
                        <div class="flex items-center gap-1">
                            <button class="w-5 h-5 bg-zinc-800 hover:bg-zinc-700 text-zinc-400 flex items-center justify-center rounded-sm text-xs border border-zinc-950 font-mono">-</button>
                            <button class="w-5 h-5 bg-zinc-800 hover:bg-zinc-700 text-zinc-400 flex items-center justify-center rounded-sm text-[10px] border border-zinc-950 font-mono">□</button>
                            <button class="w-5 h-5 bg-zinc-800 hover:bg-rose-600 hover:text-white text-zinc-400 flex items-center justify-center rounded-sm text-xs border border-zinc-950 font-mono">✕</button>
                        </div>
                    </div>

                    <!-- Terminal Menu Bar items representation -->
                    <div class="h-6 bg-zinc-900 border-b border-zinc-950 px-2 flex items-center gap-3.5 text-[11px] text-zinc-500 shrink-0 select-none">
                        <span class="hover:text-zinc-300 cursor-pointer">Terminal</span>
                        <span class="hover:text-zinc-300 cursor-pointer">Tabs</span>
                    </div>

                    <!-- Core Terminal Processing Workspace Body screen logs display panel -->
                    <div id="terminalStream" class="flex-1 p-3 overflow-y-auto terminal-font text-xs text-blue-400 space-y-2 leading-relaxed bg-black/90">
                        <div class="text-zinc-500">// XFCE Terminal Emulator Online. Native Touch & WebSerial Components Loaded.</div>
                        <div class="text-zinc-500">// Select a target vision seed from Thunar or tap/enter raw thoughts below.</div>
                        <div class="text-amber-500 font-medium">// Type 'serialconnect' to connect to an external Arduino microcontroller over USB.</div>
                        <div class="text-zinc-400">Valid commands: <span class="text-cyan-400">compile [file]</span>, <span class="text-cyan-400">ls</span>, <span class="text-cyan-400">serialconnect</span>, <span class="text-cyan-400">cat [file]</span>, <span class="text-cyan-400">clear</span></div>
                        <div class="text-zinc-700">--------------------------------------------------------------------------------</div>
                    </div>

                    <!-- Interactive TTY Input Line Form Wrapper block -->
                    <div class="border-t border-zinc-900 bg-black/95 p-2.5 flex items-center gap-1.5 shrink-0 min-h-[42px]">
                        <span id="promptPathHeader" class="terminal-font text-xs font-bold text-purple-400 shrink-0 select-none">aethon@imagination:~/visions$</span>
                        <form action="javascript:void(0);" onsubmit="submitTerminalForm()" class="flex-1 flex items-center m-0 p-0">
                            <input type="text" id="terminalPromptInput" autocomplete="off" autocorrect="off" autocapitalize="off" spellcheck="false" class="w-full bg-transparent border-none outline-none ring-0 focus:ring-0 terminal-font text-xs text-zinc-100 p-0 focus:outline-none" placeholder="Tap to express worded thoughts...">
                        </form>
                        <span class="w-2 h-4 bg-blue-400 cursor-blink shrink-0"></span>
                    </div>
                </div>

                <!-- Write Buffer Target Screen Document Panel Layer drawer box layout container -->
                <div class="h-1/2 flex flex-col xfce-window bg-zinc-900 rounded overflow-hidden">
                    <!-- Target Header Toolbar Menu section navigation tools panel -->
                    <div class="h-9 bg-zinc-950 border-b border-zinc-950 px-2 sm:px-3 flex items-center justify-between shrink-0">
                        <div class="flex items-center gap-2">
                            <span class="w-1.5 h-1.5 rounded-full bg-blue-500 shrink-0 animate-pulse"></span>
                            <span class="text-[10px] sm:text-[11px] font-mono font-bold tracking-wider text-zinc-400 truncate max-w-[130px] sm:max-w-none">VERBAL/CODE REGISTER CACHE</span>
                        </div>
                        <div class="flex items-center gap-1.5">
                            <button id="serialTransmitBtn" onclick="transmitBufferToSerial()" class="hidden text-[10px] font-mono font-bold text-amber-400 hover:text-amber-300 bg-zinc-800 hover:bg-zinc-700 border border-zinc-950 px-2.5 py-1 rounded transition flex items-center gap-1 min-h-[26px]">
                                ⚡ Flash/Stream Serial
                            </button>
                            <button onclick="copyTargetBuffer()" class="text-[10px] font-mono font-medium text-zinc-300 hover:text-white bg-zinc-800 hover:bg-zinc-700 border border-zinc-950 px-2.5 py-1 rounded transition flex items-center gap-1 min-h-[26px]">
                                Copy
                            </button>
                            <button onclick="downloadTargetBuffer()" class="text-[10px] font-mono font-medium text-emerald-400 hover:text-emerald-300 bg-zinc-800 hover:bg-zinc-700 border border-zinc-950 px-2.5 py-1 rounded transition flex items-center gap-1 min-h-[26px]">
                                Save
                            </button>
                        </div>
                    </div>
                    <!-- Content block textarea readout window pane -->
                    <div class="flex-1 relative bg-zinc-950/80 p-1">
                        <textarea id="targetOutputBuffer" readonly class="w-full h-full bg-transparent resize-none focus:outline-none p-3 text-xs text-cyan-300 leading-relaxed placeholder:text-slate-700 italic font-mono" placeholder="/* Structured vision text descriptions or compiled Arduino .ino sketches emit inside this workspace after lowering. */"></textarea>
                    </div>
                </div>

            </section>
        </div>
    </main>

    <!-- Bottom Desktop Taskbar / Window Tracker Navigation Indicator -->
    <div class="h-9 bg-zinc-900 border-t border-zinc-950 flex items-center justify-around px-2 text-xs font-mono text-zinc-400 xl:hidden shrink-0 z-50">
        <button onclick="switchMobileWindow('thunar')" id="taskbarBtnThunar" class="flex items-center gap-1.5 text-blue-400 font-bold px-3 py-1 rounded bg-zinc-950/40 border border-zinc-800/80">
            <span>📁</span> Thunar
        </button>
        <button onclick="switchMobileWindow('terminal')" id="taskbarBtnTerminal" class="flex items-center gap-1.5 px-3 py-1 rounded">
            <span>$_</span> Terminal
        </button>
    </div>

    <!-- Interactive Simulator Automation Logic Core Processing Engine -->
    <script>
        const terminalStream = document.getElementById('terminalStream');
        const promptInput = document.getElementById('terminalPromptInput');
        const targetOutput = document.getElementById('targetOutputBuffer');
        const activeFileLabel = document.getElementById('activeFileLabel');
        const thunarPathLabel = document.getElementById('thunarPathLabel');
        const fileContentGrid = document.getElementById('fileContentGrid');
        const dirCategoryHeader = document.getElementById('dirCategoryHeader');
        
        const desktopWorkspaceLabel = document.getElementById('desktopWorkspaceLabel');
        const terminalTitleLabel = document.getElementById('terminalTitleLabel');
        const promptPathHeader = document.getElementById('promptPathHeader');
        
        const hwStatusDot = document.getElementById('hwStatusDot');
        const hwStatusText = document.getElementById('hwStatusText');
        const serialTransmitBtn = document.getElementById('serialTransmitBtn');

        let currentVirtualFolder = "visions";
        let currentlySelectedFile = "";
        let hardwareSerialPort = null;
        let serialWriter = null;

        // Complete Mock Virtual File System Structure Data Model array definitions
        const virtualFileSystem = {
            'file_system': {
                path: '/',
                header: 'Root System Directories',
                items: [
                    { name: 'bin', type: 'folder', icon: '📁', target: 'fs_bin' },
                    { name: 'etc', type: 'folder', icon: '📁', target: 'fs_etc' },
                    { name: 'home', type: 'folder', icon: '📁', target: 'home' },
                    { name: 'usr', type: 'folder', icon: '📁', target: 'fs_usr' }
                ]
            },
            'fs_bin': {
                path: '/bin',
                header: 'System Executables / Binaries',
                items: [
                    { name: 'bash', type: 'binary', icon: '⚙️' },
                    { name: 'ls', type: 'binary', icon: '⚙️' },
                    { name: 'cat', type: 'binary', icon: '⚙️' },
                    { name: 'compile_engine', type: 'binary', icon: '🔮' }
                ]
            },
            'fs_etc': {
                path: '/etc',
                header: 'System Configuration Files',
                items: [
                    { name: 'aethon.conf', type: 'plain_file', icon: '📄', content: 'compiler_mode=vision_compilation\nintegrity_bounds=isolated\nactive_theme=xfce-dark-prose' },
                    { name: 'fstab', type: 'plain_file', icon: '📄', content: '/dev/sda1 / ext4 defaults 1 1\nvirtual_memory /cache tmpfs size=512M 0 0' }
                ]
            },
            'fs_usr': {
                path: '/usr',
                header: 'User Share Directory Assets',
                items: [
                    { name: 'local', type: 'folder', icon: '📁', target: 'empty_dir' },
                    { name: 'share', type: 'folder', icon: '📁', target: 'empty_dir' }
                ]
            },
            'home': {
                path: '/home',
                header: 'User Home Direct Directories',
                items: [
                    { name: 'aethon', type: 'folder', icon: '🏠', target: 'home_aethon' }
                ]
            },
            'home_aethon': {
                path: '/home/aethon',
                header: 'User Space Profiles',
                items: [
                    { name: 'Desktop', type: 'folder', icon: '🖥', target: 'desktop' },
                    { name: 'visions', type: 'folder', icon: '📂', target: 'visions' }
                ]
            },
            'desktop': {
                path: '/home/aethon/Desktop',
                header: 'Active Desktop Interface Shortcuts',
                items: [
                    { name: 'compiler_core.desktop', type: 'binary', icon: '🚀' },
                    { name: 'readme_manifest.txt', type: 'plain_file', icon: '📄', content: '== AETHON WORKSPACE MANIFEST ==\n- Fully isolated system environments\n- Decoupled Read/Write transaction caching loops active\n- Enter compile commands inside terminal frame.' }
                ]
            },
            'visions': {
                path: '/visions',
                header: 'Concept Seed Directory Files',
                items: [
                    { name: 'cyberpunk_alleyway.txt', type: 'compile_seed', icon: '📄' },
                    { name: 'steampunk_citadel.txt', type: 'compile_seed', icon: '📄' },
                    { name: 'cosmic_forge.txt', type: 'compile_seed', icon: '📄' }
                ]
            },
            'empty_dir': {
                path: '/empty_directory',
                header: 'Void Subfolder Space',
                items: []
            }
        };

        // Raw internal structured compilation textual data models map definition sets including Arduino sketches
        const workspaceFiles = {
            'cyberpunk_alleyway.txt': {
                contents: `Prompt Seed: A rain-slicked neon alleyway hiding an underground neural socket vendor deep in neo-seoul.`,
                compilations: {
                    narrative: `A heavy, violet haze choked the narrow chasm of the alleyway, split wide open by raw, shuddering neon pulses leaking from overhanging server arrays. Rainwater, slicked with iridescent synthetic motor oil, collected in deep concrete fissures, catching fractured reflections of towering sky-scrapes above. Tucked tightly between outcropping coolant pipes stood an unlabeled door, emitting a continuous low-frequency thrumming; inside, a lone tech-merchant traded black-market sub-dermal interface nodes, their silver contact points gleaming beneath flickering halide filaments.`,
                    blueprint: `ENVIRONMENT OBJECT: Sub-surface Urban Vector\nLOCATION COORD: Neo-Seoul Grid Sector 09 // Lower Slums\nATMOSPHERIC VALUES: Precipitation Index 94%, Hydro-Carbon particulates active.\nLIGHT SOURCES: Magenta-Cerulean fluorescent tube grids (unshielded, 15Hz frequency refresh rate).`,
                    poetic: `Slashes of neon weeping on cold stone,\nChrome hearts beating softly through grease and wire.`,
                    arduino: `// XFCE Compiled Reality Architecture\n// Target: Arduino Uno / Nano (Cyberpunk Alleyway Environment)\n#include <Adafruit_NeoPixel.h>\n\n#define PIXEL_PIN 6\n#define NUM_PIXELS 16\n#define COOLANT_VALVE_RELAY 4\n\nAdafruit_NeoPixel strip(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);\n\nvoid setup() {\n  Serial.begin(115200);\n  strip.begin();\n  pinMode(COOLANT_VALVE_RELAY, OUTPUT);\n  Serial.println(F("[SYSTEM]: Cyberpunk Alleyway Core Latched."));\n}\n\nvoid loop() {\n  // Simulate Shuddering Neon Pulses (Magenta / Cyan)\n  for(int i=0; i<NUM_PIXELS; i++) {\n    if(random(0,10) > 7) {\n      strip.setPixelColor(i, strip.Color(random(150,255), 0, random(150,255))); // Magenta flickering\n    } else {\n      strip.setPixelColor(i, strip.Color(0, random(180,255), random(200,255))); // Cyber Cyan base\n    }\n  }\n  strip.show();\n  \n  // Pulsing Low-Frequency coolant thrum\n  digitalWrite(COOLANT_VALVE_RELAY, (millis() % 1200 < 200) ? HIGH : LOW);\n  delay(random(50, 150));\n}`
                }
            },
            'steampunk_citadel.txt': {
                contents: `Prompt Seed: An isolated mountain fortress powered entirely by colossal high-pressure steam bellows and brass chronometers.`,
                compilations: {
                    narrative: `Clinging precariously to the jagged granite ribs of the iron-mountains stood the citadel, its vast silhouette surrounded by a perpetual billowing shroud of white, superheated steam. Colossal brass gears, each several stories tall, ground together with agonizing rhythm, sending resonant deep-iron echoes crashing into the valley valleys below.`,
                    blueprint: `STRUCTURE PROFILE: Highland Fortified Outpost\nMECHANICAL SOURCE: Geothermal pressure pockets routed into bronze piston arrays.`,
                    poetic: `The great brass lungs dilate and sigh,\nSoot blackening the borders of an ancient sky.`,
                    arduino: `// XFCE Compiled Reality Architecture\n// Target: Arduino Mega / Uno (Steampunk Citadel Engine)\n#include <Servo.h>\n\n#define BELLOWS_PULSE_PIN 9\n#define PRESSURE_GATE_RELAY 7\n#define CHRONOMETER_TICK_LED 13\n\nServo brassGearsMotor;\nunsigned long lastTick = 0;\n\nvoid setup() {\n  Serial.begin(115200);\n  brassGearsMotor.attach(BELLOWS_PULSE_PIN);\n  pinMode(PRESSURE_GATE_RELAY, OUTPUT);\n  pinMode(CHRONOMETER_TICK_LED, OUTPUT);\n  Serial.println(F("[SYSTEM]: Steampunk Citadel Piston Network Primed."));\n}\n\nvoid loop() {\n  // Drive Gear Rhythm Bellows simulation\n  for(int pos = 20; pos <= 160; pos += 2) {\n    brassGearsMotor.write(pos);\n    maintainChronometer();\n    delay(15);\n  }\n  \n  // Trigger high-pressure valve release loop safely\n  digitalWrite(PRESSURE_GATE_RELAY, HIGH);\n  delay(300);\n  digitalWrite(PRESSURE_GATE_RELAY, LOW);\n  \n  for(int pos = 160; pos >= 20; pos -= 2) {\n    brassGearsMotor.write(pos);\n    maintainChronometer();\n    delay(15);\n  }\n}\n\nvoid maintainChronometer() {\n  if(millis() - lastTick >= 1000) {\n    digitalWrite(CHRONOMETER_TICK_LED, !digitalRead(CHRONOMETER_TICK_LED));\n    Serial.println(F("[TICK]: Absolute astronomical orbit aligned."));\n    lastTick = millis();\n  }\n}`
                }
            },
            'cosmic_forge.txt': {
                contents: `Prompt Seed: A stellar forge operational within a dead star core, manifesting matter arrays directly out of solar wind streams.`,
                compilations: {
                    narrative: `Suspended directly in the dead hollow heart of a collapsed dwarf star, the forge manifested as a glowing concentric web of hyper-dense containment rings. Incandescent arcs of stellar fire whipped violently out of the surrounding cosmic void, guided into geometric containment arrays by powerful electromagnetic gravity fields.`,
                    blueprint: `ASTRO-ENGINEERING ARCHITECTURE: Core Solar Ingestion Forge\nTARGET PLATFORM: Collapsed Stellar Anchor Node\nENERGY HARVESTING: Solar radiation field capture net.`,
                    poetic: `A hollow star weeping superheated plasma tears,\nSpinning silent loops across millions of years.`,
                    arduino: `// XFCE Compiled Reality Architecture\n// Target: Arduino Architecture (Cosmic Forge Field Array Controller)\n#define MAGNETIC_COIL_PWM 5\n#define SOLAR_WIND_SENSOR A0\n#define FIELD_STABILITY_LED 11\n\nvoid setup() {\n  Serial.begin(115200);\n  pinMode(MAGNETIC_COIL_PWM, OUTPUT);\n  pinMode(FIELD_STABILITY_LED, OUTPUT);\n  Serial.println(F("[SYSTEM]: Dead Star Core Net Lock Stable."));\n}\n\nvoid loop() {\n  int solarWindInflow = analogRead(SOLAR_WIND_SENSOR);\n  \n  // Dynamically map abstract stellar wind values down into magnetic induction fields\n  int magneticFluxOutput = map(solarWindInflow, 0, 1023, 50, 255);\n  analogWrite(MAGNETIC_COIL_PWM, magneticFluxOutput);\n  \n  // Flash stability index marker dynamically\n  if(magneticFluxOutput > 200) {\n    digitalWrite(FIELD_STABILITY_LED, (millis() % 100 < 50) ? HIGH : LOW); // High stress flash\n    Serial.print(F("[WARN]: Coronal surge captured. Lattice element synthesis rate maximized: "));\n    Serial.println(solarWindInflow);\n  } else {\n    digitalWrite(FIELD_STABILITY_LED, HIGH);\n  }\n  \n  delay(200);\n}`
                }
            }
        };

        // Web Serial Hardware Interface Controller Module Routine
        async function toggleHardwarePort() {
            if (hardwareSerialPort) {
                // Safely close connection profiles
                addTerminalLine(`[SERIAL] Disconnecting from external hardware device port...`, "text-amber-500 font-mono");
                if (serialWriter) {
                    await serialWriter.close();
                    serialWriter = null;
                }
                await hardwareSerialPort.close();
                hardwareSerialPort = null;
                
                hwStatusDot.className = "w-1.5 h-1.5 rounded-full bg-amber-500";
                hwStatusText.innerText = "Arduino: Disconnected";
                serialTransmitBtn.classList.add('hidden');
                addTerminalLine(`[SERIAL] Device unlatched cleanly. Local compiler workspace back to standalone storage cache rules.`, "text-zinc-500 font-mono");
                return;
            }

            if (!("serial" in navigator)) {
                addTerminalLine(`[SERIAL ERROR] Web Serial interface drivers missing! Your browser environment doesn't possess microcontroller pipeline support layer bindings natively. (Use Chrome, Edge, or Opera over USB). Simulating virtual hardware instead.`, "text-rose-400 font-bold");
                simulateVirtualArduinoAttachment();
                return;
            }

            try {
                addTerminalLine(`[SERIAL] Prompting user hardware board assignment selection path profiles...`, "text-cyan-400 font-mono");
                hardwareSerialPort = await navigator.serial.requestPort();
                await hardwareSerialPort.open({ baudRate: 115200 });
                
                const textEncoder = new TextEncoderStream();
                textEncoder.readable.pipeTo(hardwareSerialPort.writable);
                serialWriter = textEncoder.writable.getWriter();

                hwStatusDot.className = "w-1.5 h-1.5 rounded-full bg-emerald-500 animate-pulse";
                hwStatusText.innerText = "Arduino: Connected";
                serialTransmitBtn.classList.remove('hidden');
                
                addTerminalLine(`[SERIAL SUCCESS] Handshake established with device port cleanly. Serial streams bound at 115200 Baud! Ready to stream code buffers directly down to microcontrollers.`, "text-emerald-400 font-bold");
            } catch (err) {
                addTerminalLine(`[SERIAL ERROR] Device acquisition interface exception intercepted: ${err.message}`, "text-rose-400");
                hardwareSerialPort = null;
            }
        }

        function simulateVirtualArduinoAttachment() {
            hwStatusDot.className = "w-1.5 h-1.5 rounded-full bg-blue-400 animate-pulse";
            hwStatusText.innerText = "Arduino: Simulated";
            serialTransmitBtn.classList.remove('hidden');
            hardwareSerialPort = { simulated: true };
            addTerminalLine(`[SERIAL SIM] Hooked compiler output cleanly into virtual C++ testbed bridge context. Transmit functions will trace output strings inside the shell logs.`, "text-blue-400 font-mono");
        }

        window.toggleHardwarePort = toggleHardwarePort;

        async function transmitBufferToSerial() {
            const codeStringValue = targetOutput.value;
            if (!codeStringValue) {
                addTerminalLine(`[SERIAL ERROR] Transmission terminated: Outbound text cache contains empty layout sets.`, "text-rose-400 font-mono");
                return;
            }

            addTerminalLine(`[SERIAL] Ingesting local write buffer strings... streaming rows down the active channel link...`, "text-purple-400 font-mono");
            
            if (hardwareSerialPort && hardwareSerialPort.simulated) {
                setTimeout(() => {
                    addTerminalLine(`[SERIAL SIM OUTPUT] >>> Transmitted ${codeStringValue.length} bytes to Virtual MCU interface safely.`, "text-cyan-400 font-mono italic text-[11px]");
                    addTerminalLine(`[SUCCESS] Virtual flash pipeline operation complete. Simulated hardware confirms ingestion loop activation trace.`, "text-emerald-400 font-bold");
                }, 400);
                return;
            }

            if (!serialWriter) {
                addTerminalLine(`[SERIAL ERROR] Write stream handles missing or dead. Re-assert connection layout rules.`, "text-rose-400");
                return;
            }

            try {
                // Stream chunks raw across the Web Serial USB path
                await serialWriter.write(codeStringValue + "\n");
                addTerminalLine(`[SUCCESS] Outbound buffer packet payload flushed to physical Arduino registers securely (${codeStringValue.length} text character blocks downstream). Core operating profiles synchronized.`, "text-emerald-400 font-bold underline");
            } catch (err) {
                addTerminalLine(`[SERIAL PIPE ERROR] Data ingestion packet loss encountered: ${err.message}`, "text-rose-400");
            }
        }

        window.transmitBufferToSerial = transmitBufferToSerial;

        // Android Window Swapping Logic Module Component
        window.switchMobileWindow = function(targetWindow) {
            const thunarSec = document.getElementById('windowThunar');
            const terminalSec = document.getElementById('windowTerminal');
            
            const tabT = document.getElementById('viewTabThunar');
            const tabM = document.getElementById('viewTabTerminal');
            const btnT = document.getElementById('taskbarBtnThunar');
            const btnM = document.getElementById('taskbarBtnTerminal');

            if (targetWindow === 'thunar') {
                thunarSec.className = thunarSec.className.replace('opacity-0 pointer-events-none', 'opacity-100 pointer-events-auto');
                terminalSec.className = terminalSec.className.replace('opacity-100 pointer-events-auto', 'opacity-0 pointer-events-none');
                
                tabT.className = "px-2.5 py-1 rounded-sm text-blue-400 bg-zinc-800 font-bold transition-all";
                tabM.className = "px-2.5 py-1 rounded-sm text-zinc-400 transition-all";
                btnT.className = "flex items-center gap-1.5 text-blue-400 font-bold px-3 py-1 rounded bg-zinc-950/40 border border-zinc-800/80";
                btnM.className = "flex items-center gap-1.5 px-3 py-1 rounded";
            } else {
                thunarSec.className = thunarSec.className.replace('opacity-100 pointer-events-auto', 'opacity-0 pointer-events-none');
                terminalSec.className = terminalSec.className.replace('opacity-0 pointer-events-none', 'opacity-100 pointer-events-auto');
                
                tabT.className = "px-2.5 py-1 rounded-sm text-zinc-400 transition-all";
                tabM.className = "px-2.5 py-1 rounded-sm text-blue-400 bg-zinc-800 font-bold transition-all";
                btnT.className = "flex items-center gap-1.5 px-3 py-1 rounded";
                btnM.className = "flex items-center gap-1.5 text-blue-400 font-bold px-3 py-1 rounded bg-zinc-950/40 border border-zinc-800/80";
            }
        };

        window.submitTerminalForm = function() {
            const rawVal = promptInput.value.trim();
            promptInput.value = '';
            
            if (!rawVal) return;

            const currentPath = virtualFileSystem[currentVirtualFolder].path;
            addTerminalLine(`<span class="text-purple-400">aethon@imagination:~${currentPath === '/' ? '' : currentPath}$</span> ${rawVal}`);
            processCommand(rawVal);
        };

        window.handleTerminalCommand = function(e) {
            if (e.key === 'Enter') {
                submitTerminalForm();
            }
        };

        window.navigateVirtualFolder = function(folderKey) {
            if (!virtualFileSystem[folderKey]) return;
            
            currentVirtualFolder = folderKey;
            const folderData = virtualFileSystem[folderKey];
            
            thunarPathLabel.innerText = folderData.path;
            dirCategoryHeader.innerText = folderData.header;
            
            desktopWorkspaceLabel.innerText = `Workspace: ${folderData.path}`;
            terminalTitleLabel.innerText = `aethon@imagination: ${folderData.path}`;
            promptPathHeader.innerText = `aethon@imagination:~${folderData.path === '/' ? '' : folderData.path}\$`;
            
            ['fs', 'home', 'desktop', 'visions'].forEach(id => {
                const el = document.getElementById(`sidebar-target-${id}`);
                if (el) {
                    if (id === folderKey || (folderKey.startsWith('fs_') && id === 'fs') || (folderKey.startsWith('home_') && id === 'home')) {
                        el.className = "w-full text-left flex items-center gap-1.5 text-blue-400 bg-zinc-800/60 px-1 py-1.5 rounded-sm min-h-[32px]";
                    } else {
                        el.className = "w-full text-left flex items-center gap-1.5 text-zinc-400 hover:text-zinc-200 py-1.5 px-1 rounded transition-colors min-h-[32px]";
                    }
                }
            });

            fileContentGrid.innerHTML = '';
            
            if (folderData.items.length === 0) {
                fileContentGrid.innerHTML = `<div class="text-[11px] text-zinc-600 font-mono italic p-2.5">Empty directory space</div>`;
                return;
            }

            folderData.items.forEach(item => {
                const btn = document.createElement('button');
                btn.className = "w-full text-left bg-zinc-950/40 hover:bg-zinc-800 border border-zinc-950 hover:border-zinc-700 p-2.5 rounded transition flex items-center justify-between group min-h-[38px]";
                
                let clickAction = '';
                if (item.type === 'folder') {
                    clickAction = `navigateVirtualFolder('${item.target}')`;
                } else if (item.type === 'compile_seed') {
                    clickAction = `loadFile('${item.name}')`;
                } else if (item.type === 'plain_file') {
                    clickAction = `readPlainFile('${item.name}')`;
                } else {
                    clickAction = `triggerBinaryAlert('${item.name}')`;
                }
                
                btn.setAttribute('onclick', clickAction);
                
                btn.innerHTML = `
                    <div class="flex items-center gap-2 truncate text-xs font-mono">
                        <span class="text-sm">${item.icon}</span>
                        <span class="text-zinc-300 group-hover:text-blue-400 transition-colors">${item.name}</span>
                    </div>
                    <span class="text-[9px] font-mono text-zinc-500 bg-zinc-950 px-1.5 py-0.5 rounded uppercase">${item.type}</span>
                `;
                fileContentGrid.appendChild(btn);
            });
        };

        window.loadFile = function(filename) {
            if (workspaceFiles[filename]) {
                currentlySelectedFile = filename;
                activeFileLabel.innerText = filename;
                promptInput.value = `compile ${filename}`;
                
                addTerminalLine(`[THUNAR EVENT] Loaded compilation focus context: <span class="text-amber-400">${filename}</span>`, "text-zinc-500 font-mono text-[11px]");
                
                if (window.innerWidth < 1280) {
                    setTimeout(() => switchMobileWindow('terminal'), 150);
                }
            }
        };

        function processCommand(cmdText) {
            const tokens = cmdText.split(' ').filter(t => t.trim() !== '');
            if (tokens.length === 0) return;

            const baseCmd = tokens[0].toLowerCase();

            if (baseCmd === 'clear') {
                terminalStream.innerHTML = '';
                return;
            }
            if (baseCmd === 'serialconnect') {
                toggleHardwarePort();
                return;
            }
            if (baseCmd === 'ls') {
                const items = virtualFileSystem[currentVirtualFolder].items;
                if (items.length === 0) {
                    addTerminalLine(`total 0`, "text-zinc-500 font-mono");
                    return;
                }
                const lines = items.map(i => {
                    const color = i.type === 'folder' ? 'text-blue-400 font-bold' : (i.type === 'binary' ? 'text-emerald-400' : 'text-zinc-300');
                    return `<span class="${color}">${i.name}</span>`;
                }).join('   ');
                addTerminalLine(lines);
                return;
            }
            if (baseCmd === 'cd') {
                const targetPath = tokens[1];
                if (!targetPath) {
                    navigateVirtualFolder('home');
                    return;
                }
                if (targetPath === '..') {
                    if (currentVirtualFolder.startsWith('fs_')) navigateVirtualFolder('file_system');
                    else if (currentVirtualFolder === 'home_aethon') navigateVirtualFolder('home');
                    else if (currentVirtualFolder === 'desktop' || currentVirtualFolder === 'visions') navigateVirtualFolder('home_aethon');
                    else navigateVirtualFolder('file_system');
                    return;
                }
                
                const match = virtualFileSystem[currentVirtualFolder].items.find(i => i.name === targetPath && i.type === 'folder');
                if (match && match.target) {
                    navigateVirtualFolder(match.target);
                } else {
                    addTerminalLine(`cd: no such file or directory: ${targetPath}`, "text-rose-400 font-mono");
                }
                return;
            }
            if (baseCmd === 'cat') {
                const targetFile = tokens[1];
                if (!targetFile) {
                    addTerminalLine(`cat: missing target filename sequence assignment.`, "text-rose-400 font-mono");
                    return;
                }
                if (currentVirtualFolder === 'visions' && workspaceFiles[targetFile]) {
                    addTerminalLine(`<span class="text-zinc-300 font-mono">${workspaceFiles[targetFile].contents}</span>`, "text-zinc-300");
                    return;
                }
                const itemMatch = virtualFileSystem[currentVirtualFolder].items.find(i => i.name === targetFile && i.type === 'plain_file');
                if (itemMatch && itemMatch.content) {
                    addTerminalLine(`<pre class="text-zinc-300 font-mono whitespace-pre-wrap">${itemMatch.content}</pre>`);
                } else {
                    addTerminalLine(`cat: ${targetFile}: File target unmapped inside folder bounds.`, "text-rose-400 font-mono");
                }
                return;
            }
            if (baseCmd === 'compile') {
                const targetFile = tokens[1];
                if (!targetFile || !workspaceFiles[targetFile] || currentVirtualFolder !== 'visions') {
                    compileDirectPrompt(cmdText.substring(8));
                    return;
                }
                runFileCompilation(targetFile);
            } else {
                compileDirectPrompt(cmdText);
            }
        }

        function compileDirectPrompt(customText) {
            addTerminalLine(`[COMPILER] Ingesting abstract semantic word parameters: "${customText}"`, "text-cyan-400 font-mono");
            addTerminalLine(`[COMPILER] Resolving text structural frameworks... connected logic arrays safely.`, "text-zinc-500 font-mono");
            
            setTimeout(() => {
                const format = document.getElementById('targetCompilationFormat').value;
                if (format === 'arduino') {
                    targetOutput.value = `// XFCE Compiled Vision Hardware Layout Sketch\n// Input prompt target: "${customText}"\n\nvoid setup() {\n  Serial.begin(115200);\n  Serial.println("Vision Core Online: ${customText}");\n}\n\nvoid loop() {\n  // Output streams map dynamic thoughts down directly into operational registers\n  Serial.println("Bringing imagination to life through text...");\n  delay(5000);\n}`;
                } else {
                    targetOutput.value = `/**\n * AUTONOMOUS WORD COMPILATION OUTPUT PASS\n * CUSTOM VISION PARAMETER: "${customText}"\n */\n\nA beautiful tapestry of written words materializes out of the abstract dark workspace boundaries, shaping your prompt targets cleanly into physical sentences. Every conceptual description loops seamlessly across the narrative layers, bringing the dynamic visions of your imagination directly into a stable text structure.`;
                }
                addTerminalLine(`[SUCCESS] Custom concept compiled cleanly into cache register targets.`, "text-emerald-400 font-bold");
            }, 500);
        }

        function runFileCompilation(filename) {
            const format = document.getElementById('targetCompilationFormat').value;
            const fileData = workspaceFiles[filename];

            addTerminalLine(`[COMPILER] Processing file entity target pass: <span class="text-zinc-300">${filename}</span>`, "text-purple-400 font-mono");
            addTerminalLine(`[COMPILER] Parsing markers... lower abstraction layers mapped.`, "text-zinc-500 font-mono");
            
            setTimeout(() => {
                addTerminalLine(`[COMPILER] Output channel verified: <span class="text-cyan-400">.${format === 'arduino' ? 'ino' : format}</span>`, "text-zinc-500 font-mono");
                
                setTimeout(() => {
                    targetOutput.value = fileData.compilations[format];
                    addTerminalLine(`[SUCCESS] XFCE Compiler lowering sequence finished cleanly. Ready to download or transmit.`, "text-emerald-400 font-bold text-[11px]");
                }, 400);
            }, 300);
        }

        window.copyTargetBuffer = function() {
            const txt = targetOutput.value;
            if (!txt) return;

            const dummyArea = document.createElement("textarea");
            dummyArea.value = txt;
            dummyArea.style.position = "fixed";
            document.body.appendChild(dummyArea);
            dummyArea.select();

            try {
                document.execCommand('copy');
                addTerminalLine(`[TTY SYSTEM] Output data storage successfully copied to clipboard registers.`, "text-cyan-400 font-mono text-[10px]");
            } catch (err) {
                addTerminalLine(`[TTY ERROR] Clipboard bridge execution exception intercepted.`, "text-rose-400");
            }

            document.body.removeChild(dummyArea);
        };

        window.downloadTargetBuffer = function() {
            const val = targetOutput.value;
            if (!val) {
                addTerminalLine(`[TTY ERROR] Aborting save: Output text box is void.`, "text-rose-400");
                return;
            }

            const format = document.getElementById('targetCompilationFormat').value;
            let ext = 'txt';
            if (format === 'narrative') ext = 'prose';
            else if (format === 'blueprint') ext = 'spec';
            else if (format === 'poetic') ext = 'verse';
            else if (format === 'arduino') ext = 'ino';

            try {
                const elementBlob = new Blob([val], { type: 'text/plain;charset=utf-8' });
                const blobUrlRef = URL.createObjectURL(elementBlob);
                
                const hiddenAnchorElement = document.createElement('a');
                hiddenAnchorElement.href = blobUrlRef;
                hiddenAnchorElement.download = `xfce_compiled_vision.${ext}`;
                
                document.body.appendChild(hiddenAnchorElement);
                hiddenAnchorElement.click();
                
                document.body.removeChild(hiddenAnchorElement);
                URL.revokeObjectURL(blobUrlRef);

                addTerminalLine(`[TTY SYSTEM] File structure saved securely as: 'xfce_compiled_vision.${ext}'`, "text-purple-400 font-mono text-[10px]");
            } catch (err) {
                addTerminalLine(`[TTY ERROR] Local storage exception: ${err.message}`, "text-rose-400");
            }
        };

        window.readPlainFile = function(filename) {
            let content = "Empty descriptor data.";
            Object.values(virtualFileSystem).forEach(f => {
                const match = f.items.find(i => i.name === filename);
                if (match && match.content) content = match.content;
            });

            addTerminalLine(`[THUNAR EVENT] Opened plain text layout view stream for '${filename}':`, "text-zinc-400 font-mono text-[11px]");
            addTerminalLine(`<pre class="bg-zinc-900/60 p-2 rounded border border-zinc-950 text-zinc-300 font-mono text-[11px] whitespace-pre-wrap">${content}</pre>`);
            if (window.innerWidth < 1280) {
                setTimeout(() => switchMobileWindow('terminal'), 150);
            }
        };

        window.triggerBinaryAlert = function(binaryName) {
            addTerminalLine(`[SHELL ERROR] Access Denied: elevated environment parameters required.`, "text-rose-400 font-mono text-[11px]");
            if (window.innerWidth < 1280) {
                setTimeout(() => switchMobileWindow('terminal'), 150);
            }
        };

        window.onload = function() {
            navigateVirtualFolder('visions');
            if (window.innerWidth < 1280) {
                switchMobileWindow('thunar');
            } else {
                document.getElementById('windowThunar').style.opacity = "1";
                document.getElementById('windowThunar').style.pointerEvents = "auto";
                document.getElementById('windowTerminal').style.opacity = "1";
                document.getElementById('windowTerminal').style.pointerEvents = "auto";
            }
        };

        window.addEventListener('resize', function() {
            if (window.innerWidth >= 1280) {
                document.getElementById('windowThunar').style.opacity = "1";
                document.getElementById('windowThunar').style.pointerEvents = "auto";
                document.getElementById('windowTerminal').style.opacity = "1";
                document.getElementById('windowTerminal').style.pointerEvents = "auto";
            } else {
                const thunarActive = document.getElementById('viewTabThunar').classList.contains('text-blue-400');
                switchMobileWindow(thunarActive ? 'thunar' : 'terminal');
            }
        });
    </script>
</body>
</html>
