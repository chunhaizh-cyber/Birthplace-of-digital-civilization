import fs from "node:fs/promises";
import path from "node:path";
import { spawn } from "node:child_process";

import { McpServer } from "@modelcontextprotocol/sdk/server/mcp.js";
import { StdioServerTransport } from "@modelcontextprotocol/sdk/server/stdio.js";
import { z } from "zod";

const WORKSPACE = "D:\\鱼巢";
const EXE_PATH = path.join(WORKSPACE, "x64", "Debug", "鱼巢.exe");
const BUILD_LOCK_PATH = path.join(WORKSPACE, ".codex_locks", "build-run.lock");

function textResult(text) {
  return { content: [{ type: "text", text }] };
}

function tailText(text, maxChars = 6000) {
  if (!text) {
    return "";
  }
  if (text.length <= maxChars) {
    return text;
  }
  return `...<trimmed ${text.length - maxChars} chars>\n${text.slice(-maxChars)}`;
}

async function pathExists(targetPath) {
  try {
    await fs.access(targetPath);
    return true;
  } catch {
    return false;
  }
}

async function ensureUnlocked(action) {
  if (!(await pathExists(BUILD_LOCK_PATH))) {
    return;
  }
  let raw = "";
  try {
    raw = await fs.readFile(BUILD_LOCK_PATH, "utf8");
  } catch {
    raw = "(build-run.lock exists but could not be read)";
  }
  throw new Error(`Refusing to ${action}: build-run.lock is present.\n${raw}`);
}

function runProcess(command, args, options = {}) {
  return new Promise((resolve) => {
    const child = spawn(command, args, {
      cwd: options.cwd ?? WORKSPACE,
      env: { ...process.env, ...(options.env ?? {}) },
      shell: false,
      windowsHide: true
    });

    let stdout = "";
    let stderr = "";

    child.stdout.on("data", (chunk) => {
      stdout += chunk.toString();
    });

    child.stderr.on("data", (chunk) => {
      stderr += chunk.toString();
    });

    child.on("error", (error) => {
      resolve({
        ok: false,
        exitCode: null,
        signal: null,
        stdout,
        stderr,
        error: String(error)
      });
    });

    child.on("close", (code, signal) => {
      resolve({
        ok: code === 0,
        exitCode: code,
        signal,
        stdout,
        stderr,
        error: null
      });
    });
  });
}

function formatProcessResult(title, command, args, result) {
  const lines = [
    `${title}`,
    `command: ${command} ${args.join(" ")}`,
    `exit_code: ${result.exitCode === null ? "null" : result.exitCode}`,
    `signal: ${result.signal ?? "null"}`
  ];

  if (result.error) {
    lines.push(`error: ${result.error}`);
  }

  if (result.stdout) {
    lines.push("", "[stdout]", tailText(result.stdout));
  }

  if (result.stderr) {
    lines.push("", "[stderr]", tailText(result.stderr));
  }

  return lines.join("\n");
}

async function ensureExeExists() {
  if (await pathExists(EXE_PATH)) {
    return;
  }
  throw new Error(`Executable not found: ${EXE_PATH}`);
}

const server = new McpServer({
  name: "project-scripts-mcp",
  version: "0.1.0"
});

server.registerTool(
  "check_specs",
  {
    title: "check_specs",
    description: "Run python .\\tools\\check_specs.py in D:\\鱼巢.",
    inputSchema: {
      strict: z.boolean().optional().default(false)
    }
  },
  async ({ strict }) => {
    const args = [".\\tools\\check_specs.py"];
    if (strict) {
      args.push("--strict");
    }
    const result = await runProcess("python", args);
    return textResult(formatProcessResult("check_specs", "python", args, result));
  }
);

server.registerTool(
  "build_debug_x64",
  {
    title: "build_debug_x64",
    description: "Run the standard Debug x64 msbuild command for D:\\鱼巢.",
    inputSchema: {}
  },
  async () => {
    await ensureUnlocked("build");
    const args = [
      ".\\鱼巢.vcxproj",
      "/p:Configuration=Debug",
      "/p:Platform=x64",
      "/p:LinkIncremental=false",
      "/m"
    ];
    const result = await runProcess("msbuild", args);
    return textResult(formatProcessResult("build_debug_x64", "msbuild", args, result));
  }
);

server.registerTool(
  "run_panel_html",
  {
    title: "run_panel_html",
    description: "Run x64\\Debug\\鱼巢.exe --panel-html in D:\\鱼巢.",
    inputSchema: {}
  },
  async () => {
    await ensureUnlocked("run panel-html");
    await ensureExeExists();
    const args = ["--panel-html"];
    const result = await runProcess(EXE_PATH, args);
    return textResult(formatProcessResult("run_panel_html", EXE_PATH, args, result));
  }
);

server.registerTool(
  "run_self_check_log",
  {
    title: "run_self_check_log",
    description: "Run x64\\Debug\\鱼巢.exe --self-check-log with an optional wait time.",
    inputSchema: {
      wait_ms: z.number().int().min(0).max(120000).optional().default(10000)
    }
  },
  async ({ wait_ms }) => {
    await ensureUnlocked("run self-check-log");
    await ensureExeExists();
    const args = ["--self-check-log", `--self-check-log-wait-ms=${wait_ms}`];
    const result = await runProcess(EXE_PATH, args);
    return textResult(formatProcessResult("run_self_check_log", EXE_PATH, args, result));
  }
);

server.registerTool(
  "git_snapshot",
  {
    title: "git_snapshot",
    description: "Return a compact local Git snapshot for D:\\鱼巢.",
    inputSchema: {}
  },
  async () => {
    const commands = [
      ["git", ["status", "--short", "--branch", "--untracked-files=no"]],
      ["git", ["log", "-5", "--oneline"]],
      ["git", ["diff", "--stat"]]
    ];

    const outputs = [];
    for (const [command, args] of commands) {
      const result = await runProcess(command, args);
      outputs.push(formatProcessResult(command, command, args, result));
    }

    const revList = await runProcess("git", ["rev-list", "--left-right", "--count", "HEAD...birthplace/main"]);
    outputs.push(formatProcessResult("git rev-list", "git", ["rev-list", "--left-right", "--count", "HEAD...birthplace/main"], revList));

    return textResult(outputs.join("\n\n"));
  }
);

const transport = new StdioServerTransport();
await server.connect(transport);
